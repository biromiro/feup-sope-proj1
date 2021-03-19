#include "../include/permission_caller.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "../include/dirs.h"
#include "../include/file_status.h"
#include "../include/logger.h"
#include "../include/process.h"
#include "../include/signals.h"

int handle_change_mods(cmd_args_t *args, char *argv[], char *envp[]) {
    struct stat status;
    int err;

    for (size_t i = args->files_start; i < args->files_end; i++) {
        update_file_pinfo(argv[i]);
        lock_process();
        if (get_status(argv[i], &status) != 0) {
            fprintf(stderr,
                    "xmod: cannot access '%s': No such file or directory\n",
                    argv[i]);
            continue;
        }

        if ((err = change_perms(argv[i], args, &status)) != 0) {
            fprintf(stderr,
                    "xmod: %s\n", strerror(err));
            return err;
        }

        lock_process();
        if (args->options.recursive && is_dir(&status)) {
            // printf("folder: %s\n", argv[i]);
            if ((err = recursive_change_mod(argv[i], args, argv, envp)) != 0) {
                fprintf(stderr,
                        "xmod: %s\n", strerror(err));
                return err;
            }
        } else {
            // printf("file: %s\n", argv[i]);
        }
    }

    return 0;
}

int change_perms(const char *pathname, cmd_args_t *args, struct stat *status) {
    int res;

    mode_t current_permission = get_access_perms(status), new_permission = 0;

    get_new_perms(pathname, &args->mode, current_permission, &new_permission);

    // Always verbose, needs change accounting for options

    if ((res = chmod(pathname, new_permission))) {
        perror("ERROR CALLING CHMOD");
        return res;
    }

    update_file_status_pinfo(current_permission != new_permission);

    print_chmod_call(current_permission, new_permission, pathname, args);

    write_permission_log(pathname, current_permission, new_permission);
    return 0;
}

int get_new_perms(const char *pathname,
                  const perm_operation_t *const permissions,
                  mode_t current_perm, mode_t *new_perm) {
    int permission_types[3] = {permissions->permission_types.type_o,
                               permissions->permission_types.type_g,
                               permissions->permission_types.type_u};

    for (int offset = 0; offset < 3; offset++) {
        // current permissions for read, write and execute

        int octal_offset = get_octal_offset(offset),
            permission_digit = (current_perm / octal_offset) % OCTAL_BASE;

        int current_r = (permission_digit / READ_VAL) * READ_VAL,
            rest_r = permission_digit % READ_VAL;
        int current_w = (rest_r / WRITE_VAL) * WRITE_VAL,
            rest_w = rest_r % WRITE_VAL;
        int current_x = (rest_w / EXEC_VAL);

        // new permissions for read, write and execute

        permission_digit = ((permissions->permission_octal) / get_octal_offset(offset)) % OCTAL_BASE;

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

        *new_perm += (new_r + new_w + new_x) * octal_offset;
    }
    return 0;
}

int get_octal_offset(int offset) {
    int res = 1;
    for (int i = 1; i < offset + 1; i++) {
        res *= OCTAL_BASE;
    }

    return res;
}

int print_chmod_call(mode_t current_permission, mode_t new_permission,
                     const char *pathname, const cmd_args_t *const args) {
    char new_perm_string[10], cur_perm_string[10];
    get_permission_string(new_permission, new_perm_string,
                          sizeof(new_perm_string));
    get_permission_string(current_permission, cur_perm_string,
                          sizeof(cur_perm_string));

    if (args->options.verbose && current_permission == new_permission) {
        printf("mode of '%s' retained as %04o (%s)\n", pathname, new_permission,
               new_perm_string);
    } else if ((args->options.verbose || args->options.verbose_on_modify) &&
               current_permission != new_permission) {
        printf("mode of '%s' changed from %04o (%s) to %04o (%s)\n", pathname,
               current_permission, cur_perm_string, new_permission,
               new_perm_string);
    }

    return 0;
}

void get_permission_string(mode_t permission, char *perm_string,
                           size_t buffer_size) {
    char *permission_substrings[8] = {"---", "--x", "-w-", "-wx",
                                      "r--", "r-x", "rw-", "rwx"};

    char permission_octal[5] = "";

    octal_to_string(permission, permission_octal);

    size_t digit1 = permission_octal[1] - '0',
           digit2 = permission_octal[2] - '0',
           digit3 = permission_octal[3] - '0';

    snprintf(perm_string, buffer_size, "%s%s%s", permission_substrings[digit1],
             permission_substrings[digit2], permission_substrings[digit3]);
}

void octal_to_string(mode_t octal, char *output) {
    int size_string = 5;  // 4 digits + null terminator

    snprintf(output, size_string, "%o", octal);
    snprintf(output, size_string, "%.*d%o",
             (int)(size_string - strlen(output) - 1),
             0, octal);
}
