#ifndef INCLUDE_ARGS_PARSER_H_
#define INCLUDE_ARGS_PARSER_H_

#include <sys/stat.h>
#include <stddef.h>
#include <stdbool.h>

#include "error/exit_codes.h"

#define ERROR_FLAG -1
#define READ_VAL 4
#define WRITE_VAL 2
#define EXEC_VAL 1

#define VALID_USER(user) \
    (user == 'u' || user == 'g' || user == 'o' || user == 'a')
#define VALID_PERM(perm) (perm == '=' || perm == '+' || perm == '-')

#define USAGE "USAGE: xmod [-vcR] MODE/OCTAL file/dir"

/**
 * Structure containing the information about the various command options
 */
typedef struct cmd_options {
    bool verbose;
    bool verbose_on_modify;
    bool recursive;
} cmd_options_t;

/**
 * Enumerates the different changes to a user's type
 */
typedef enum { ADD, REMOVE, SUBSTITUTE, UNCHANGED } perm_change_type;

/**
 * Defines the type of permission change to make to each user
 */
typedef struct {
    perm_change_type type_u;
    perm_change_type type_g;
    perm_change_type type_o;
} perm_changes_t;

/**
 * Defines a permission operation.
 *
 * A permission operation is given by its octal value, as well as the type of
 * change for each user type (since it might not want to SUBSTITUTE values not
 * specified), as well as its validity.
 */
typedef struct {
    mode_t permission_octal;
    perm_changes_t permission_types;
} perm_operation_t;

/**
 * Struct containing all the information about the command arguments
 */
typedef struct cmd_args {
    cmd_options_t options;
    perm_operation_t mode;
    size_t files_start;
    size_t files_end;
} cmd_args_t;

/**
 * Parses the command line arguments of the command.
 *
 * @param cmd_args_t the command arguments options
 * @param argc the number of command line arguments
 * @param argv the array of the command line arguments
 * @param envp the environment variables
 */
int parse_args(cmd_args_t* args, int argc, char* argv[], char* envp[]);

/**
 * Constructor of a perm_changes_t object.
 *
 * @param type_u the 'user' permission change type
 * @param type_g the 'group' permission change type
 * @param type_o the 'other' permission change type
 */

perm_changes_t create_perm_changes_t(perm_change_type type_u,
                                     perm_change_type type_g,
                                     perm_change_type type_o);

/**
 * Parsed the xmod mode given by the command line arguments.
 *
 * @param mode the mode to parse
 */
int parse_mode(char* mode, perm_operation_t* perms);

/**
 * Parses a given mode in octal into a perm_operation_t object.
 *
 * @param mode the mode to parse
 * @param type the permission change types for the users
 */
int parse_octal_mode(char* mode, perm_changes_t type, perm_operation_t* perms);

/**
 * Parses a given mode given in text mode into a perm_operation_t object.
 *
 * @param mode the mode to parse
 */
int parse_text_mode(char* mode, perm_operation_t* perms);

/**
 * Reads a defined user permission change and returns its value.
 *
 * @param user_perms the user permissions to parse and read
 */
char parse_user_type_perms(char* user_perms);

/**
 * Gets the total sum for the permission of a given user.
 *
 * @param perms the permissions to calculate
 */
char get_sum_perms(char* perms);

#endif  // INCLUDE_ARGS_PARSER_H_
