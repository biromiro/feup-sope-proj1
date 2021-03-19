#ifndef INCLUDE_FILE_STATUS_H_
#define INCLUDE_FILE_STATUS_H_

#include <sys/stat.h>
#include <stdbool.h>

/**
 * @brief Gets the status of the file.
 *
 * @param[in] pathname string containing the pathname of the file.
 * @param[out] status pointer to a stat struct that will store information of
 *the file.
 *
 * @return an error value.
 **/
int get_status(const char* const pathname, struct stat* status);

/**
 * @brief Gets the lstatus of the file.
 *
 * @param[in] pathname string containing the pathname of the file.
 * @param[out] status pointer to a stat struct that will store information of
 *the file.
 *
 * @return an error value.
 **/
int get_lstatus(const char* const pathname, struct stat* status);

/**
 * @brief Checks if a file is a directory, given its status.
 *
 * @param[in] status const pointer to a stat struct that contains information on
 *the file.
 *
 * @return true if file is a directory.
 **/
bool is_dir(const struct stat* status);

/**
 * @brief Checks if a file is a link, given its status (only works when status
 *is obtained with lstat).
 *
 * @param[in] status const pointer to a stat struct that contains information on
 *the file.
 *
 * @return true if file is a link.
 **/
bool is_slink(const struct stat* status);

/**
 * @brief Gets the access permissions of a file, given its status.
 *
 * @param[in] status const pointer to a stat struct that contains information on
 *the file.
 *
 * @return the access permissions as an integer.
 **/
int get_access_perms(const struct stat* status);

#endif  // INCLUDE_FILE_STATUS_H_
