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
 * @param[in] envp env given to xmod.
 * @param[in] new_path path of the dir being opened.
 *
 * @return an error value.
 **/
int try_enter_dir(DIR* directory, cmd_args_t* args, char* argv[], char* envp[],
                  char* new_path) {
    fflush(NULL);  // flush the output buffer so that printf doesn't
                   // print both on parent and on child process
    lock_process();
    int id = fork();
    if (id == -1) {
        closedir(directory);
        perror("fork error");
        return errno;
    }

    if (id == 0) {
        closedir(directory);

        setup_argv(args, argv, new_path);
        write_process_create_log(4,
                                 argv);  // PROC_CREAT needs to be between
                                         // fork() and lock_process(), otherwise
                                         // the event would not be logged.
        fflush(NULL);                    // ensure it prints before it dies

        lock_process();
        return execve("xmod", argv, envp);
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

        if (pid > 0) {  // if there was child not caught by sigchld
            if (WIFSIGNALED(w_status)) {
                snprintf(out, sizeof(out), "%d", -WTERMSIG(w_status));
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

    // printf("IN %s-------\n", pathname);
    struct stat status;
    int err;

    if (get_status(pathname, &status)) {
        return errno;
    }

    DIR* directory = opendir(pathname);

    if (directory == NULL) {
        perror("ERROR WHILE OPENING DIRECTORY");
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
        // printf("%s\n", new_path);
        // printf("CUR DIR: %s length %d", newPath, directory_entry->d_reclen);

        if (get_status(new_path, &status)) {
            closedir(directory);
            return errno;
        }

        // printf(" is dir: %d access mode: %o\n", is_dir(&status),
        //       get_access_perms(&status));

        // lock_process();

        update_file_pinfo(new_path);

        if (is_dir(&status)) {
            while (
                (err = try_enter_dir(directory, args, argv, envp, new_path))) {
                if (err != UNJUST_CHILD_DEATH) {
                    return err;
                }
            }
        } else {
            lock_process();
            if (change_perms(new_path, args, &status) != 0) {
                closedir(directory);
                perror("ERROR WHILE CHANGING PERMISSION!");
                return errno;
            }
        }

        errno = 0;
    }

    if (errno != 0) {
        closedir(directory);
        perror("ERROR GETTING NEXT DIR");
        return errno;
    }

    if (closedir(directory)) {
        perror("ERROR WHILE CLOSING DIR");
        return errno;
    }

    lock_process();

    // printf("LEAVING %s----------\n", pathname);

    return 0;
}
