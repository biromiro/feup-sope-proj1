#define LOG_ENV_VAR "LOG_FILENAME"

/**
 * @brief Opens the log file, if one is given, and gets its file descriptor
 *number.
 *
 * @param[out] file_descriptor the integer file descriptor returned.
 *
 * @return NO_FILE_GIVEN if no log file was set or different than 0 if an error
 *occured
 **/
int open_log(int* file_descriptor);
