#include "file_status.h"
#include <bits/stdint-uintn.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int open_dir(const char* pathname, uint8_t depth) {
    DIR* directory = opendir(pathname);

    if (directory == NULL) {
        perror("ERROR WHILE OPENING DIRECTORY");
        return errno;
    }

    printf("IN %s-------\n", pathname);

    struct dirent* directory_entry;
    struct stat status;
    char newPath[(depth + 1) * MAXNAMLEN + 2];

    errno = 0;
    while ((directory_entry = readdir(directory)) != NULL) {
        if (!strcmp(directory_entry->d_name, "..")) continue;

        snprintf(newPath, 2 * MAXNAMLEN + 2, "%s/%s", pathname,
                 directory_entry->d_name);
        printf("CUR DIR: %s length %d", newPath, directory_entry->d_reclen);

        if (get_status(newPath, &status)) {
            return errno;
        }

        printf(" is dir: %d access mode: %o\n", is_dir(&status),
               get_access_perms(&status));

        if (is_dir(&status) && strcmp(directory_entry->d_name, ".")) {
            if (open_dir(newPath, depth + 1)) {
                return errno;
            }
        }
    }

    if (errno != 0) {
        perror("ERROR GETTING NEXT DIR");
        return errno;
    }

    if (closedir(directory)) {
        perror("ERROR WHILE CLOSING DIR");
        return errno;
    }

    printf("LEAVING %s----------\n", pathname);

    return 0;
}