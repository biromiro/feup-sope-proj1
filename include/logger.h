#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <stdbool.h>

#include "../include/permission_caller.h"

enum Event { PROC_CREAT = 0,
             PROC_EXIT,
             SIGNAL_RECV,
             SIGNAL_SENT,
             FILE_MODF };


/**
 * @brief Initializes the log_info struct.
 **/
void init_log_info();

/**
 * @brief Checks if logging is enabled.
 *
 * @return true if logging is enabled.
 **/
bool is_logging();

/**
 * @brief Opens the log file, if one is given, and gets its file descriptor
 *number.
 *
 * @return NO_FILE_GIVEN if no log file was set or different than 0 if an error
 *occured
 **/
int open_log();

/**
 * @brief Writes into the log using a
 *format and variable arguments.
 *
 * @param[in] format string format same as printf.
 * @param[in] ... Arguments for format specification.
 *
 * @return an error value.
 **/
int write_log_format(const char* format, ...);

/**
 * @brief Writes into the log with the predefined format.
 *
 * @param event event enum specifying the event being logged.
 * @param[in] info string containing information about the event.
 *
 * @return an error value.
 **/
int write_log(enum Event event, const char* info);

/**
 * @brief Closes the log with file descriptor file_descriptor.
 *
 * @return an error value.
 **/
int close_log();

int write_permission_log(const char* pathname,
                         mode_t current_permission,
                         mode_t new_permission);

int write_process_create_log(int argc, char* argv[]);

int write_signal_recv_log(int signo);

int write_signal_send_group_log(int pid, int signo);

int write_signal_send_process_log(int pid, int signo);

#endif  // INCLUDE_LOGGER_H_
