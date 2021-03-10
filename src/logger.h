#include <time.h>
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

/**
 * @brief Writes into the log with file descriptor file_descriptor using a
 *format and variable arguments.
 *
 * @param file_descriptor the integer file descriptor.
 * @param format string format same as printf.
 * @param[in] ... Arguments for format specification.
 *
 * @return an error value.
 **/
int write_log_format(int file_descriptor, const char* format, ...);

/**
 * @brief Writes into the log with file descriptor file_descriptor 
 *
 * @param file_descriptor the integer file descriptor.
 * @param format string format same as printf.
 * @param[in] ... Arguments for format specification.
 *
 * @return an error value.
 **/
int write_log(int file_descriptor, time_t instant, int pid,
              const char* action_event, const char* info);

/**
 * @brief Closes the log with file descriptor file_descriptor.
 *
 * @param file_descriptor the integer file descriptor.
 *
 * @return an error value.
 **/
int close_log(int file_descriptor);

