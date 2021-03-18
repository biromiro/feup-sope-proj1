#ifndef INCLUDE_AUX_H_
#define INCLUDE_AUX_H_

#include <sys/types.h>

typedef u_int64_t clock_ms_t;

/**
 * @brief Waits for a char from stdin and cleans buffer
 * 
 * @return character from keyboard
 */
int get_clean_char();

#endif  // INCLUDE_AUX_H_
