#ifndef INCLUDE_PROCESS_H_
#define INCLUDE_PROCESS_H_

#include <stdbool.h>
#include <sys/types.h>

/**
 * Strcture containing valuable information about the process
 */
typedef struct process_info {
    char *curr_file;
    pid_t child_pid;
    int files_found;
    int files_changed;
} pinfo_t;

/**
 * @brief Setups process info
 */
void setup_pinfo();

/**
 * @brief Updates process info
 * 
 * @param path file/folder currently being analysed. Send NULL for unchanged
 */
void update_file_pinfo(const char *path);

/**
 * @brief Updates process info
 * 
 * @param pid pid of child, if any
 */
void update_pid_pinfo(pid_t info);

/**
 * @brief Updates files count
 * 
 * @param changed if the current file was changed
 */
void update_file_status_pinfo(bool changed);

/**
 * @brief Gets process info
 *
 * @return process info
 */
pinfo_t *get_pinfo();

#endif  // INCLUDE_PROCESS_H_
