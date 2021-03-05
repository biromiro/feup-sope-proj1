#include "args_parser.h"

int parse_args(cmd_args_t *args, int argc, char *argv[], char *envp[]) {
    args->recursive = true;
    args->verbose = false;
    args->verbose_on_modidy = true;
    perm_operation val = parse_mode(argv[1]);
    if (val.invalid)
        printf("something's wrong!");
    else
        printf("%o", val.permission_octal);
    return 0;
}

perm_changes create_perm_changes(perm_change_type type_u, perm_change_type type_g, perm_change_type type_o) {
    perm_changes permissions;

    permissions.type_u = type_u;
    permissions.type_g = type_g;
    permissions.type_o = type_o;

    return permissions;
}

perm_operation parse_mode(char *mode) {
    char *endptr;
    strtol(mode, &endptr, 8);
    if (*endptr != '\0') {
        return parse_text_mode(mode);
    } else {
        return parse_octal_mode(mode, create_perm_changes(SUBSTITUTE, SUBSTITUTE, SUBSTITUTE));
    }
}

perm_operation parse_octal_mode(char *mode, perm_changes types) {
    perm_operation perms;
    mode_t code = strtol(mode, NULL, 8);
    if (errno == EINVAL) {
        perms.invalid = true;
        return perms;
    } else {
        perms.permission_octal = code;
        perms.permission_types = types;
        perms.invalid = false;
        return perms;
    }
}

perm_operation parse_text_mode(char *mode) {
    char octal_val[5] = "0000";
    char *begin_of_perm = strtok_r(mode, ",", &mode);

    perm_change_type type_u = UNCHANGED, type_g = UNCHANGED, type_o = UNCHANGED;

    while (begin_of_perm != NULL) {
        char new_octal_val = parse_user_type_perms(begin_of_perm);
        if (new_octal_val == ERROR_FLAG) begin_of_perm[0] = ' ';  //goes to default
        switch (begin_of_perm[0]) {
            case 'u':
                octal_val[1] = new_octal_val;
                type_u = (begin_of_perm[1] == '=' ? SUBSTITUTE : (begin_of_perm[1] == '+' ? ADD : REMOVE));
                break;
            case 'g':
                octal_val[2] = new_octal_val;
                type_g = (begin_of_perm[1] == '=' ? SUBSTITUTE : (begin_of_perm[1] == '+' ? ADD : REMOVE));
                break;
            case 'o':
                octal_val[3] = new_octal_val;
                type_o = (begin_of_perm[1] == '=' ? SUBSTITUTE : (begin_of_perm[1] == '+' ? ADD : REMOVE));
                break;
            default: {
                perm_operation perms;
                perms.invalid = true;
                return perms;
            }
        }
        begin_of_perm = strtok_r(NULL, ",", &mode);
    }
    return parse_octal_mode(octal_val, create_perm_changes(type_u, type_g, type_o));
}

char parse_user_type_perms(char *user_perms) {
    if (strlen(user_perms) < 3 || (user_perms[1] != '=' && user_perms[1] != '+' && user_perms[1] != '-')) return ERROR_FLAG;
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