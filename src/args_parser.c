#include "args_parser.h"

/**
 * @brief Parse the options
 * The options are optional. This function will fill a cmd_args_t with valuable information.
 * In case of success returns 0 and if an option is not valid, returns the invalida character used.
 * It also returns the index for the next parser to evaluate
 * 
 * @param [out] args struct to return information about the options
 * @param [out] last_arg index of the the argument for the next parser to evaluate
 * @param argc
 * @param argv   
 */
int parse_options(cmd_args_t *args, int *last_arg, int argc, char *argv[]) {
    const char *valid_args = "vcRrwx";
    *last_arg = 1;
    for (size_t i = 1; i < argc; i++) {
        const char *str = argv[i];
        const size_t str_size = strlen(str);
        char curr[] = "";
        if (str[0] == '-') {
            for (size_t j = 1; j < str_size; j++) {
                curr[0] = str[j];

                // Verifies if it is a valid flag, even if it's from options
                // Shouldn't give an error on xmod -v -x file for example
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
                            args->verbose_on_modify = !args->verbose;
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

    int opt, index;
    if ((opt = parse_options(args, &index, argc, argv)) != 0) {
        fprintf(stderr,
                "xmod: invalid mode: '-%c'\n", opt);
        exit(BAD_OPTION);
    }
    printf("Current index: %d -> %s\n", index, argv[index]);

    return 0;
}
