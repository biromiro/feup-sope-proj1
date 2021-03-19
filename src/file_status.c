#include "../include/file_status.h"

#include <stdio.h>
#include <errno.h>

int get_status(const char* const pathname, struct stat* status) {
    int err;

    if ((err = stat(pathname, status))) {
        perror("ERROR GETTING STATUS");
        return errno;
    }

    return 0;
}

int get_lstatus(const char* const pathname, struct stat* status) {
    int err;

    if((err = lstat(pathname, status))) {
        perror("ERROR GETTING LSTATUS");
        return errno;
    }

    return 0;
}

bool is_dir(const struct stat* status) { return S_ISDIR(status->st_mode); }

bool is_slink(const struct stat* status) {
    return S_ISLNK(status->st_mode);
}  // it will never be a link, stat returns status of linked file, lstat could
   // report that a file is slink but lstat is now useful in this program

int get_access_perms(const struct stat* status) {
    return status->st_mode & ACCESSPERMS;
}
