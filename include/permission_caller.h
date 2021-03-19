#ifndef INCLUDE_PERMISSION_CALLER_H_
#define INCLUDE_PERMISSION_CALLER_H_

#include "../include/args_parser.h"

#define OCTAL_BASE 8

/**
 * @brief Handles the chmod call and takes into account the options given
 * 
 * @param[in] args the parsed arguments given
 * @param[in] argv the command line arguments
 * 
 * @return 0 upon sucess, an error flag otherwise
 */
int handle_change_mods(cmd_args_t *args, char *argv[], char *envp[]);

/**
 * @brief Changes the permissions of a given file
 * 
 * @param[in] pathname the path of the file to change
 * @param[in] permissions the permissions to change the file to
 * @param[in] status the current file status
 * 
 * @return 0 upon sucess, an error flag otherwise
 */
int change_perms(const char *pathname, cmd_args_t *args, struct stat *status);

/**
 * @brief Calculates the new permissions in octal
 * 
 * @param[in] pathname the path of the file to change
 * @param[in] permissions the permissions to change the file to
 * @param[in] current_perm the current permissions of the file
 * @param[out] new_perm the new permissions of the file (to return)
 * 
 * @return 0 upon sucess, an error flag otherwise
 */
int get_new_perms(const char *pathname,
                  const perm_operation_t *const permissions,
                  mode_t current_perm,
                  mode_t *const new_perm);

/**
 * @brief Gets the offset of the number in octal given its position. 
 * In decimal, the 2nd digit has an offset of 10. In octal, it has an offset of 8.
 * This function facilitates this operation.
 * 
 * @param[in] offset the position of the digit to get the offset of
 * 
 * @return the corresponding offset in octal
 */
int get_octal_offset(int offset);

/**
 * @brief Prints the chmod call information according to the options given
 * 
 * @param[in] current_permission the permission the file currently has
 * @param[in] new_permission the new permission given to the file
 * @param[in] pathname the path to the file
 * @param[in] args the arguments given
 * 
 * @return 0 upon sucess, an error flag otherwise
 */
int print_chmod_call(mode_t current_permission,
                     mode_t new_permission,
                     const char *pathname,
                     const cmd_args_t *const args);

/**
 * @brief Gets the permission string in rwx terms for each group
 * 
 * @param[in] permission the permission to calculate the string for
 * @param[out] perm_string the permission string
 * @param[in] buffer_size the size of the string
 */
void get_permission_string(mode_t permission,
                           char *perm_string, size_t buffer_size);

/**
 * @brief Converts an octal permission to a 4 digit string
 * 
 * @param[in] octal the octal permission
 * @param[out] output the string to store the octal permission
 */
void octal_to_string(mode_t octal, char *output);

#endif  // INCLUDE_PERMISSION_CALLER_H_
