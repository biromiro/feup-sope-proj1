#include "../include/permission_caller.h"

#include <math.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../include/file_status.h"

int change_perms(const char *pathname, perm_operation_t *permissions) {
    mode_t new_permission = 0;
    get_new_perms(pathname, permissions, &new_permission);
    return chmod(pathname, new_permission);
}

int get_new_perms(const char *pathname, perm_operation_t *permissions, mode_t *new_perm) {
    int err;
    struct stat status;

    if ((err = get_status(pathname, &status)) != 0) return err;

    int current_perms = get_access_perms(&status);

    int permission_types[3] = {permissions->permission_types.type_o,
                               permissions->permission_types.type_g,
                               permissions->permission_types.type_u};

    printf("Should be 0: %o\n", *new_perm);

    for (int offset = 0; offset < 3; offset++) {
        //current permissions for read, write and execute

        int permission_digit = (current_perms / get_octal_offset(offset)) % OCTAL_BASE;

        printf("current perms = %o, permission_digit = %o\n", current_perms, permission_digit);

        int current_r = (permission_digit / READ_VAL) * READ_VAL, rest_r = permission_digit % READ_VAL;
        int current_w = (rest_r / WRITE_VAL) * WRITE_VAL, rest_w = rest_r % WRITE_VAL;
        int current_x = (rest_w / EXEC_VAL);

        printf("current_r = %o, current_w = %o, current_x = %o\n\n", current_r, current_w, current_x);

        //new permissions for read, write and execute

        printf("permission = %o\n", permissions->permission_octal);

        permission_digit = ((permissions->permission_octal) / get_octal_offset(offset)) % OCTAL_BASE;

        printf("permission digit = %d\n\n", permission_digit);

        int new_r = (permission_digit / READ_VAL) * READ_VAL;
        rest_r = permission_digit % READ_VAL;
        int new_w = (rest_r / WRITE_VAL) * WRITE_VAL;
        rest_w = rest_r % WRITE_VAL;
        int new_x = (rest_w / EXEC_VAL);

        switch (permission_types[offset]) {
            case ADD:
                new_r = (current_r == 0) ? new_r : current_r;
                new_w = (current_w == 0) ? new_w : current_w;
                new_x = (current_x == 0) ? new_x : current_x;
                break;
            case REMOVE:
                new_r = (new_r == 0) ? current_r : 0;
                new_w = (new_w == 0) ? current_w : 0;
                new_x = (new_x == 0) ? current_x : 0;
                break;
            case UNCHANGED:
                new_r = current_r;
                new_w = current_w;
                new_x = current_x;
                break;
            default:
                break;
        }

        printf("permission_types[offset] = %d, new_r = %d, new_w = %d, new_x = %d\n", permission_types[offset], new_r, new_w, new_x);
        printf("%o\n\n", (new_r + new_w + new_x) * get_octal_offset(offset));
        *new_perm += (new_r + new_w + new_x) * get_octal_offset(offset);
    }
    printf("%o\n", *new_perm);
    return 0;
}

int get_octal_offset(int offset) {
    int res = 1;
    for (int i = 1; i < offset + 1; i++) {
        res *= OCTAL_BASE;
    }

    return res;
}