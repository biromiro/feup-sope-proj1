#ifndef INCLUDE_AUX_H_
#define INCLUDE_AUX_H_

#include <stdbool.h>
#include <sys/types.h>

typedef u_int64_t clock_ms_t;

/**
 * @brief Waits for a char from stdin and cleans buffer
 *
 * @return character from keyboard
 */
int get_clean_char();

/**
 * @brief Gets the exit code if it was set using set_and_exit().
 *
 * @return the exit code.
 */
int get_exit_code();

/**
 * @brief Used to check if an exit log should be written.
 *
 * @return true if no log should be written.
 */
bool is_silent_exit();

/**
 * @brief Sets the exit code to be logged and calls exit.
 *
 * @param exit_c the exit code.
 */
void set_and_exit(int exit_c);

#endif  // INCLUDE_AUX_H_
