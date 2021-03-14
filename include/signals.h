#ifndef INCLUDE_SIGS_H_
#define INCLUDE_SIGS_H_

#define ERR_SIGACTION 2
#define ERR_SIGEMPTYMASK 3

#include <stdbool.h>

/**
 * @brief Setups signal handlers
 *
 * @return 0 if sucessfull, 2 if error on sigaction, 3 if error on sigemptymask
 */
int setup_handlers();

/**
 * @brief Verifies if process is waiting for user feedback
 *
 * @return process waiting for user feedback
 */
bool is_waiting();

/**
 * @brief Locks the process and waits for children to finish their process
 */
void lock_wait_process();

/**
 * @brief Locks the process while waiting for user feedback
 */
void lock_process();

int sig_log_register(int signo);

#endif  // INCLUDE_SIGS_H
