#ifndef INCLUDE_PERMISSION_CALLER_H_
#define INCLUDE_PERMISSION_CALLER_H_

#include "args_parser.h"

#define OCTAL_BASE 8

int change_perms(const char *pathname, perm_operation_t *permissions);

int get_new_perms(const char *pathname, perm_operation_t *permissions, mode_t *new_permission);

int get_octal_offset(int offset);

#endif  // INCLUDE_PERMISSION_CALLER_H_
