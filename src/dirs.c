#include <bits/stdint-uintn.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/signals.h"
#include "file_status.h"

/**
 * @brief Changes the permissions of all files inside a directory, recursively
 *opening directories inside it.
 *
 * @param[in] pathname string containing the pathname of the directory.
 * @param depth depth of the recursion.
 *
 * @return an error value.
 **/
int recursive_change_mod_inner(const char* pathname, uint8_t depth) {
    // used find ..  -printf '%M %p\n' | wc -l, and  ./xmod .. | wc -l, to test
    // if this func works correctly
    DIR* directory = opendir(pathname);

    if (directory == NULL) {
        perror("ERROR WHILE OPENING DIRECTORY");
        return errno;
    }

    // printf("IN %s-------\n", pathname);
    // <<<<<<<<< change permission of dir here

    struct dirent* directory_entry;
    struct stat status;
    char newPath[(depth + 1) * MAXNAMLEN + 2];

    errno = 0;
    while ((directory_entry = readdir(directory)) != NULL) {
        if (!strcmp(directory_entry->d_name, "..") ||
            !strcmp(directory_entry->d_name, "."))
            continue;

        snprintf(newPath, (depth + 1) * MAXNAMLEN + 2, "%s/%s", pathname,
                 directory_entry->d_name);
        printf("%s\n", newPath);
        // printf("CUR DIR: %s length %d", newPath, directory_entry->d_reclen);

        if (get_status(newPath, &status)) {
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
                perror("fork error");
                return errno;
            }

            if (id == 0) {
                lock_process();
                if (recursive_change_mod_inner(newPath, depth + 1)) exit(errno);
                exit(0);
            } else {
                lock_wait_process();
            }
        } else {
            // <<<<<<<<<<< change permission of file here
        }
    }

    // if (errno != 0) {
    //     perror("ERROR GETTING NEXT DIR");
    //     return errno;
    // }

    if (closedir(directory)) {
        perror("ERROR WHILE CLOSING DIR");
        return errno;
    }

    // printf("LEAVING %s----------\n", pathname);

    return 0;
}

int recursive_change_mod(const char* pathname) {
    return recursive_change_mod_inner(pathname, 0);
}
