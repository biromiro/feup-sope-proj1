#include <stdint.h>

/**
 * @brief Changes the permissions of all files inside a directory, recursively
 *opening directories inside it. Creates a process for each directory.
 *
 * @param[in] pathname string containing the pathname of the directory.
 *
 * @return an error value.
 **/
int recursive_change_mod(const char* pathname);