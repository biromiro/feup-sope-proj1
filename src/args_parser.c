#include "args_parser.h"

int parse_args(cmd_args_t *args, int argc, char *argv[], char *envp[]) {
    args->recursive = true;
    args->verbose = false;
    args->verbose_on_modidy = true;
    mode_t val = parse_mode(argv[1]);
    if (val == ERROR_FLAG)
        printf("something's wrong!");
    else
        printf("%o", val);
    return 0;
}

mode_t parse_mode(char *mode) {
    char *endptr;
    strtol(mode, &endptr, 8);
    if (*endptr != '\0') {
        printf("text mode!\n");
        return parse_text_mode(mode);
    } else {
        return parse_octal_mode(mode);
    }
}

mode_t parse_octal_mode(char *mode) {
    mode_t code = strtol(mode, NULL, 8);
    if (errno == EINVAL)
        return ERROR_FLAG;
    else
        return code;
}

mode_t parse_text_mode(char *mode) {
    printf("hello");
    char octal_val[5];
    char *begin_of_perm = strtok(mode, ",");
    //printf("%s", begin_of_perm);
    while (begin_of_perm != NULL) {
        char *curr_str = malloc(sizeof(char) * strlen(begin_of_perm));
        sprintf(curr_str, "%s", begin_of_perm);
        //printf("%s", curr_str);
        char new[1];
        new[0] = parse_user_type_perms(curr_str);

        switch (curr_str[0]) {
            case 'u':
                octal_val[1] = new[0];
                break;
            case 'g':
                octal_val[2] = new[0];
                break;
            case 'o':
                octal_val[3] = new[0];
                break;
            default:
                return -1;
        }

        strcat(octal_val, new);
        free(curr_str);
        begin_of_perm = strtok(NULL, ",");
    }

    octal_val[0] = '0';
    octal_val[4] = '\0';
    return parse_octal_mode(octal_val);
}

char parse_user_type_perms(char *user_perms) {
    if (strlen(user_perms) < 3 || user_perms[1] != '=') return ERROR_FLAG;

    char *begin_of_perm = strtok(user_perms, "=");
    if (user_perms[0] == 'u' || user_perms[0] == 'g' || user_perms[0] == 'o') {
        begin_of_perm = strtok(NULL, "=");
        return get_sum_perms(begin_of_perm);
    } else
        return ERROR_FLAG;
}

char get_sum_perms(char *perms) {
    int sum = 0;

    for (int i = 0; i < strlen(perms); ++i) {
        switch (perms[i]) {
            case 'r':
                sum += READ_VAL;
                break;
            case 'w':
                sum += WRITE_VAL;
                break;
            case 'x':
                sum += EXEC_VAL;
                break;
            default:
                return ERROR_FLAG;
        }
    }

    return sum + '0';
}