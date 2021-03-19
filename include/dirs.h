#ifndef INCLUDE_DIRS_H_
#define INCLUDE_DIRS_H_

#include <stdint.h>

#include "../include/args_parser.h"

/**
 * @brief Changes the permissions of all files inside a directory, recursively
 *opening directories inside it.
 *
 * @param[in] pathname string containing the pathname of the directory.
 * @param[in] args the arguments of the call
 *
 * @return an error value.
 **/
int recursive_change_mod(const char* pathname, cmd_args_t* args, char* argv[],
                         char* envp[]);

#endif  // INCLUDE_DIRS_H_
