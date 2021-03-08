#include "args_parser.h"

int parse_args(cmd_args_t *args, int argc, char *argv[], char *envp[]) {
    args->recursive = true;
    args->verbose = false;
    args->verbose_on_modidy = true;
    perm_operation val;
    int res = parse_mode(argv[1], &val);
    if (res == ERROR_FLAG)
        printf("something's wrong!");
    else {
        printf("%o, %d, %d, %d\n", val.permission_octal, val.permission_types.type_u, val.permission_types.type_g, val.permission_types.type_o);
    }
    return 0;
}

perm_changes create_perm_changes(perm_change_type type_u,
                                 perm_change_type type_g,
                                 perm_change_type type_o) {
    perm_changes permissions;

    permissions.type_u = type_u;
    permissions.type_g = type_g;
    permissions.type_o = type_o;

    return permissions;
}

perm_change_type parse_perm_change_type(char del) {
    switch (del) {
        case '=':
            return SUBSTITUTE;
        case '-':
            return REMOVE;
        case '+':
            return ADD;
        default:
            return UNCHANGED;
            break;
    }
}

int parse_mode(char *mode, perm_operation *perm) {
    if (!isdigit(mode[0])) {
        return parse_text_mode(mode, perm);
    } else {
        return parse_octal_mode(
            mode,
            create_perm_changes(SUBSTITUTE, SUBSTITUTE, SUBSTITUTE),
            perm);
    }
}

int parse_octal_mode(char *mode, perm_changes types, perm_operation *perms) {
    char *end;
    mode_t code = strtol(mode, &end, 8);
    if (*end != '\0' || errno == EINVAL) {
        return ERROR_FLAG;
    } else {
        perms->permission_octal = code;
        perms->permission_types = types;
    }
    return 0;
}

int parse_text_mode(char *mode, perm_operation *perms) {
    char octal_val[5] = "0000";
    char *begin_of_perm = strtok_r(mode, ",", &mode);

    perm_change_type type_u = UNCHANGED, type_g = UNCHANGED, type_o = UNCHANGED;

    while (begin_of_perm != NULL) {
        char user = begin_of_perm[0], delimiter = begin_of_perm[1];
        char new_octal_val = parse_user_type_perms(begin_of_perm);

        if (new_octal_val == ERROR_FLAG) return ERROR_FLAG;

        switch (user) {
            case 'u':
                octal_val[1] = new_octal_val;
                type_u = parse_perm_change_type(delimiter);
                break;
            case 'g':
                octal_val[2] = new_octal_val;
                type_g = parse_perm_change_type(delimiter);
                break;
            case 'o':
                octal_val[3] = new_octal_val;
                type_o = parse_perm_change_type(delimiter);
                break;
            case 'a':
                octal_val[1] = new_octal_val;
                octal_val[2] = new_octal_val;
                octal_val[3] = new_octal_val;
                type_u = parse_perm_change_type(delimiter);
                type_g = type_u;
                type_o = type_u;

                break;
            default: {
                return ERROR_FLAG;
            }
        }
        begin_of_perm = strtok_r(NULL, ",", &mode);
    }
    return parse_octal_mode(
        octal_val, create_perm_changes(type_u, type_g, type_o), perms);
}

char parse_user_type_perms(char *user_perms) {
    char delimiter = user_perms[1], user = user_perms[0];
    char delimiter_string[2];

    sprintf(delimiter_string, "%c", delimiter);

    if (strlen(user_perms) >= 3 &&
        VALID_PERM(delimiter) &&
        VALID_USER(user)) {
        char *perms = strtok_r(user_perms, delimiter_string, &user_perms);
        perms = strtok_r(NULL, delimiter_string, &user_perms);
        return get_sum_perms(perms);
    }
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