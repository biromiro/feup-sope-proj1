#include "file_status.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int open_dir(const char* pathname) {
    DIR* directory = opendir(pathname);

    if (directory == NULL) {
        perror("ERROR WHILE OPENING DIRECTORY");
        return errno;
    }

    printf("IN %s-------\n", pathname);

    struct dirent* directory_entry;
    struct stat status;
    char newPath[2 * MAXNAMLEN + 2];

    errno = 0;
    while ((directory_entry = readdir(directory)) != NULL) {
        snprintf(newPath, 2 * MAXNAMLEN + 2, "%s/%s", pathname,
                 directory_entry->d_name);
        printf("CUR DIR: %s length %d", newPath, directory_entry->d_reclen);

        if (get_status(newPath, &status)) {
            return errno;
        }

        printf(" is dir: %d access mode: %o\n", is_dir(&status),
               get_access_perms(&status));

    }

    if (errno != 0) {
        perror("ERROR GETTING NEXT DIR");
        return errno;
    }

    if (closedir(directory)) {
        perror("ERROR WHILE CLOSING DIR");
        return errno;
    }

    return 0;
}