#include "logger.h"
#include <string.h>

const char* get_log_filename(char* envp[]) {
    char* env_token;
    char* env_var;
    unsigned i = 0;

    while ((env_var = envp[i]) != NULL) {
        env_token = strtok(env_var, "=");

        if (strcmp(env_token, LOG_ENV_VAR) == 0) {
            return strtok(NULL, "=");
        }

        i++;
    }

    return NULL;
}

int open_log(const char* pathname, int* file_descriptor) { return 0; }
