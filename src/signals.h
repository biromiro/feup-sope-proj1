#ifndef INCLUDE_SIGS_H_
#define INCLUDE_SIGS_H_

#define ERR_SIGACTION 2
#define ERR_SIGEMPTYMASK 3

/**
 * @brief Setups signal handlers
 *
 * @return 0 if sucessfull, 2 if error on sigaction, 3 if error on sigemptymask
 */
int setup_handlers();

int sig_log_register(int signo);

#endif  // INCLUDE_SIGS_H
