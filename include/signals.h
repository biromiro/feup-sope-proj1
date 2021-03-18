#ifndef INCLUDE_SIGNALS_H_
#define INCLUDE_SIGNALS_H_

#define ERR_SIGACTION 2
#define ERR_SIGEMPTYMASK 3

#define NO_OVERRIDE_SIG(s) \
    (s != SIGKILL && s != SIGSTOP && s != 32 && s != 33 && s != SIGUSR1)

#include <stdbool.h>

/**
 * @brief Setups signal handlers
 *
 * @return 0 if sucessfull, 2 if error on sigaction, 3 if error on sigemptymask
 */
int setup_handlers();

/**
 * @brief Frees resources used by signal handlers
 *
 * @return 0 if sucessfull.
 */
void unsetup_handlers();

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
 * @brief Locks the process while waiting for user feedback on interruption, or
 * tries to check if a signal has triggered and handles it.
 */
void lock_process();

/**
 * @brief Registers to the log a signal received
 *
 * @param signo Signal identifier
 */
int sig_log_register(int signo);

#endif  // INCLUDE_SIGNALS_H_
