#include "../include/dirs.h"

#include <asm-generic/errno-base.h>
#include <bits/stdint-uintn.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/file_status.h"
#include "../include/permission_caller.h"
#include "../include/process.h"
#include "../include/signals.h"
#include "../include/logger.h"

extern char** environ;

void setup_argv(cmd_args_t* args, char* argv[], char* new_path) {
    argv[args->files_start] = new_path;
    if (args->files_end > args->files_start + 1)
        argv[args->files_start + 1] = 0;
}

/**
 * @brief Tries to create a process that changes the access modifiers of the
 *directory
 *
 * @param[in] directory pointer to the DIR being opened.
 * @param[in] args
 * @param[in] argv arguments given to xmod.
 * @param[in] new_path path of the dir being opened.
 *
 * @return an error value.
 **/
int try_enter_dir(DIR* directory, cmd_args_t* args, char* argv[],
                  char* new_path) {
    fflush(NULL);  // flush the output buffer so that printf doesn't
                   // print both on parent and on child process
    lock_process();
    int id = fork();
    reset_handlers();  // after fork child has the same pipe
                       // descriptors as parent, this lessens the
                       // chance of interference

    if (id == 0) {
        closedir(directory);

        setup_argv(args, argv, new_path);
        write_process_create_log(4,
                                 argv);  // PROC_CREAT needs to be between
                                         // fork() and lock_process(), otherwise
                                         // the event would not be logged.
        fflush(NULL);                    // ensure it prints before it dies

        lock_process();
        if (execve("xmod", argv, environ)) {
            return errno;
        }
    } else if (id == -1) {
        closedir(directory);
        return errno;
    } else {
        update_pid_pinfo(id);
        pid_t pid;
        int w_status;
        char out[255];

        errno = 0;
        do {
            lock_process();
        } while ((pid = wait(&w_status)) == -1 && errno == EINTR);
        // while the result of wait isn't interrupted by a system
        // call

        if (pid > 0) {
            if (WIFSIGNALED(w_status)) {
                snprintf(out, sizeof(out), "%d (interrupted)",
                         -WTERMSIG(w_status));
                write_log(PROC_EXIT, out);

                return UNJUST_CHILD_DEATH;  // meaning child was killed before
                                            // SIGINT handlers were set or by
                                            // TERM, KILL, etc..
            }

            fflush(NULL);
        } else if (pid < 0 && errno == ECHILD) {
            return UNJUST_CHILD_DEATH;
        }
    }

    return 0;
}

/**
 * @brief Changes the permissions of all files inside a directory, recursively
 *opening directories inside it.
 *
 * @param[in] pathname string containing the pathname of the directory.
 *
 * @return an error value.
 **/
int recursive_change_mod(const char* pathname, cmd_args_t* args, char* argv[],
                         char* envp[]) {
    // used find ..  -printf '%M %p\n' | wc -l, and  ./xmod .. | wc -l, to test
    // if this func works correctly
    struct stat status;
    int err;

    DIR* directory = opendir(pathname);

    if (directory == NULL) {
        return errno;
    }

    struct dirent* directory_entry;
    const size_t kPath_size = (strlen(pathname)) + MAXNAMLEN + 2;
    char new_path[kPath_size];
    lock_process();
    while ((directory_entry = readdir(directory)) != NULL) {
        if (!strcmp(directory_entry->d_name, "..") ||
            !strcmp(directory_entry->d_name, "."))
            continue;

        snprintf(new_path, kPath_size, "%s/%s", pathname,
                 directory_entry->d_name);

        if (get_lstatus(new_path, &status)) {
            closedir(directory);
            return errno;
        }

        if (is_slink(&status)) {
            printf("neither symbolic link '%s' nor referent has been changed\n",
                   new_path);
            continue;
        }

        update_file_pinfo(new_path);

        if (is_dir(&status)) {
            while ((err = try_enter_dir(directory, args, argv, new_path))) {
                if (err != UNJUST_CHILD_DEATH) {
                    break;
                }
            }
        } else {
            lock_process();
            if (change_perms(new_path, args, &status) != 0) {
                if (errno == EACCES || errno == EPERM || errno == EROFS) {
                    dprintf(STDERR_FILENO,
                            "xmod: changing permissions of '%s: %s\n", new_path,
                            strerror(errno));
                    if (args->options.verbose) print_fail_call(new_path, args);

                    continue;
                }

                closedir(directory);
                return errno;
            }
        }

        errno = 0;
    }

    if (errno != 0) {
        closedir(directory);
        return errno;
    }

    if (closedir(directory)) {
        return errno;
    }

    lock_process();

    return 0;
}
