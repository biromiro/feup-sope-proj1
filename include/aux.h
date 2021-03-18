#ifndef INCLUDE_AUX_H_
#define INCLUDE_AUX_H_

#include <sys/types.h>
#include <time.h>

typedef u_int64_t clock_ms_t;

/**
 * @brief Waits for a char from stdin and cleans buffer
 *
 * @return character from keyboard
 */
int get_clean_char();

/**
 * @brief Gets the current time in milliseconds since epoch
 */
void read_curr_time_ms(clock_ms_t* clock_val);

/**
 * @brief Converts integer to a string.
 *
 * @param num the integer.
 * @param[out] str the resulting string.
 * @param buffer_size size of the string pointer.
 *
 * @return size of the resulting string
 */
int int_to_string(int num, char* str, int buffer_size);

#endif  // INCLUDE_AUX_H_
