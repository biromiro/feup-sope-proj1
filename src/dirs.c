#include "../include/dirs.h"

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
#include "../include/signals.h"

/**
 * @brief Changes the permissions of all files inside a directory, recursively
 *opening directories inside it.
 *
 * @param[in] pathname string containing the pathname of the directory.
 * @param depth depth of the recursion.
 *
 * @return an error value.
 **/

int recursive_change_mod(const char* pathname,
                         cmd_args_t* args,
                         char* argv[],
                         char* envp[]) {
    // used find ..  -printf '%M %p\n' | wc -l, and  ./xmod .. | wc -l, to test
    // if this func works correctly

    // printf("IN %s-------\n", pathname);
    struct stat status;

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

    errno = 0;
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

        lock_process();

        if (is_dir(&status)) {
            fflush(NULL);  // flush the output buffer so that printf doesn't
                           // print both on parent and on child process
            int id = fork();
            if (id == -1) {
                closedir(directory);
                perror("fork error");
                return errno;
            }

            if (id == 0) {
                closedir(directory);

                argv[args->files_start] = new_path;
                if (args->files_end > args->files_start + 1)
                    argv[args->files_start + 1] = 0;

                lock_process();
                return execve("xmod", argv, envp);
            } else {
                lock_wait_process();
            }
        } else {
            if (change_perms(new_path, args, &status) != 0) {
                closedir(directory);
                perror("ERROR WHILE CHANGING PERMISSION!");
                return errno;
            }
        }
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

    // printf("LEAVING %s----------\n", pathname);

    return 0;
}
