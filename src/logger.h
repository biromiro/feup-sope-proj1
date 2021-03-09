#define LOG_ENV_VAR "LOG_FILENAME"

const char* get_log_filename(char* envp[]);

int open_log(const char* pathname, int* file_descriptor);