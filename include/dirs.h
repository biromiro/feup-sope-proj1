#ifndef INCLUDE_DIRS_H_
#define INCLUDE_DIRS_H_

#include "args_parser.h"

/**
 * @brief Changes the permissions of all files inside a directory, recursively
 *opening directories inside it. Creates a process for each directory.
 *
 * @param[in] pathname string containing the pathname of the directory.
 * @param permissions the struct contating the permissions to change.
 * @param args the arguments of the call
 *
 * @return an error value.
 **/
int recursive_change_mod(const char* pathname, cmd_args_t* args);


/**
 * @brief Changes the permissions of all files inside a directory, recursively
 *opening directories inside it.
 *
 * @param[in] pathname string containing the pathname of the directory.
 * @param depth depth of the recursion.
 * @param args the arguments of the call
 *
 * @return an error value.
 **/
int recursive_change_mod_inner(const char* pathname, unsigned short depth, cmd_args_t* args);

#endif  // INCLUDE_DIRS_H_
