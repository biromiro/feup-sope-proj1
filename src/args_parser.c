#include "args_parser.h"

/**
 * @brief Parse the options
 * The options are optional. This function will fill a cmd_args_t with valuable information.
 * In case of success returns 0 and if an option is not valid, returns the invalida character used.
 * It also returns the index for the next parser to evaluate
 * 
 * @param [out] args struct to fill information about the options
 * @param [out] last_arg index of the the argument for the next parser to evaluate
 * @param argc
 * @param argv   
 */
int parse_options(cmd_options_t *args, int *last_arg, int argc, char *argv[]) {
    const char *valid_args = "vcR";
    *last_arg = 1;
    for (size_t i = 1; i < argc; i++) {
        const char *str = argv[i];
        const size_t str_size = strlen(str);
        char curr[] = "";
        // Options can be in multiple strings: -v -R is valid
        if (str[0] == '-') {
            for (size_t j = 1; j < str_size; j++) {
                curr[0] = str[j];

                // Options should override previous values that are in conflict
                // -cv: v should override c
                // -vc: c should override v
                if (strstr(valid_args, curr) == NULL) {
                    return str[j];
                } else {
                    switch (str[j]) {
                        case 'R':
                            args->recursive = true;
                            break;
                        case 'v':
                            args->verbose = true;
                            args->verbose_on_modify = false;
                            break;
                        case 'c':
                            args->verbose = false;
                            args->verbose_on_modify = true;
                            break;
                        default:
                            // Last arg will be set to the current argv index
                            // It can be a mode flag that will be analysed
                            // in the next parser
                            return 0;
                            break;
                    }
                }
            }
        } else {
            return 0;
        }

        (*last_arg)++;
    }
    return 0;
}

int parse_args(cmd_args_t *args, int argc, char *argv[], char *envp[]) {
    memset(args, 0, sizeof(cmd_args_t));

    int opt, next_op_idx;
    if ((opt = parse_options(&args->options, &next_op_idx, argc, argv)) != 0) {
        fprintf(stderr,
                "xmod: invalid option: '-%c'\n%s\n", opt, USAGE);
        exit(BAD_OPTION);
    }

    if (next_op_idx >= argc) {
        fprintf(stderr,
                "xmod: missing MODE\n%s\n", USAGE);
        exit(BAD_ARGS);
    }

    if (parse_mode(argv[next_op_idx], &args->mode) != 0) {
        fprintf(stderr,
                "xmod: invalid mode: '%s'\n%s\n", argv[next_op_idx], USAGE);
        exit(BAD_MODE);
    }

    args->files_start = next_op_idx + 1;
    args->files_end = argc;
    return 0;
}

perm_changes_t create_perm_changes(perm_change_type type_u,
                                   perm_change_type type_g,
                                   perm_change_type type_o) {
    perm_changes_t permissions;

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

int parse_mode(char *mode, perm_operation_t *perm) {
    int res;
    char *param = malloc(strlen(mode));
    strcpy(param, mode);
    if (!isdigit(param[0])) {
        res = parse_text_mode(param, perm);
    } else {
        res = parse_octal_mode(
            param,
            create_perm_changes(SUBSTITUTE, SUBSTITUTE, SUBSTITUTE),
            perm);
    }
    free(param);
    return res;
}

int parse_octal_mode(char *mode, perm_changes_t types, perm_operation_t *perms) {
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

int parse_text_mode(char *mode, perm_operation_t *perms) {
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

    char used[4] = "";

    for (int i = 0; i < strlen(perms); ++i) {
        switch (perms[i]) {
            case 'r':
                if (strstr(used, "r") == NULL) {
                    sum += READ_VAL;
                    strcat(used, "r");
                }
                break;
            case 'w':
                if (strstr(used, "w") == NULL) {
                    sum += WRITE_VAL;
                    strcat(used, "w");
                }
                break;
            case 'x':
                if (strstr(used, "x") == NULL) {
                    sum += EXEC_VAL;
                    strcat(used, "x");
                }
                break;
            default:
                return ERROR_FLAG;
        }
    }

    return sum + '0';
}