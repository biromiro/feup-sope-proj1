#ifndef INCLUDE_PERMISSION_CALLER_H_
#define INCLUDE_PERMISSION_CALLER_H_

#include "args_parser.h"

#define OCTAL_BASE 8

int handle_change_mods(cmd_args_t *args, char* argv[]);

/**
 * @brief Changes the permissions of a given file
 * 
 * @param pathname the path of the file to change
 * @param permissions the permissions to change the file to
 * 
 * @return 0 upon sucess, an error flag otherwise
 */
int change_perms(const char *pathname, cmd_args_t *args);

/**
 * @brief Calculates the new permissions in octal
 * 
 * @param pathname the path of the file to change
 * @param permissions the permissions to change the file to
 * @param current_perm the current permissions of the file
 * @param new_perm the new permissions of the file (to return)
 * 
 * @return 0 upon sucess, an error flag otherwise
 */
int get_new_perms(const char *pathname, perm_operation_t *permissions, mode_t current_perm, mode_t *new_perm);

/**
 * @brief Gets the offset of the number in octal given its position. 
 * In decimal, the 2nd digit has an offset of 10. In octal, it has an offset of 8.
 * This function facilitates this operation.
 * 
 * @param offset the position of the digit to get the offset of
 * 
 * @return the corresponding offset in octal
 */
int get_octal_offset(int offset);

int print_chmod_call(mode_t current_permission, mode_t new_permission, const char * pathname, cmd_args_t *args);

const char* get_permission_string(mode_t permission);

#endif  // INCLUDE_PERMISSION_CALLER_H_
