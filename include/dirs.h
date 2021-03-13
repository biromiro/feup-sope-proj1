#ifndef INCLUDE_DIRS_H_
#define INCLUDE_DIRS_H_

/**
 * @brief Changes the permissions of all files inside a directory, recursively
 *opening directories inside it. Creates a process for each directory.
 *
 * @param[in] pathname string containing the pathname of the directory.
 *
 * @return an error value.
 **/
int recursive_change_mod(const char* pathname);

#endif  // INCLUDE_DIRS_H_
