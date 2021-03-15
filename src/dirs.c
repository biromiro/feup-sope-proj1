#include "../include/dirs.h"

#include <bits/stdint-uintn.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/file_status.h"
#include "../include/permission_caller.h"

int recursive_change_mod_inner(const char* pathname, uint16_t depth, cmd_args_t* args) {
    // used find ..  -printf '%M %p\n' | wc -l, and  ./xmod .. | wc -l, to test
    // if this func works correctly

    // printf("IN %s-------\n", pathname);
    if (change_perms(pathname, args) != 0) {
        perror("ERROR WHILE CHANGING PERMISSION!");
        return errno;
    }

    DIR* directory = opendir(pathname);

    if (directory == NULL) {
        perror("ERROR WHILE OPENING DIRECTORY");
        return errno;
    }

    struct dirent* directory_entry;
    struct stat status;
    const size_t kPath_size = (depth + 2) * MAXNAMLEN + 2;
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
                if (recursive_change_mod_inner(new_path, depth + 1, args)) {
                    closedir(directory);
                    exit(errno);
                }
                closedir(directory);
                exit(0);
            }
        } else {
            if (change_perms(new_path, args) != 0) {
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

int recursive_change_mod(const char* pathname, cmd_args_t* args) {
    return recursive_change_mod_inner(pathname, 0, args);
}
