#ifndef INCLUDE_PROCESS_H_
#define INCLUDE_PROCESS_H_

#include <sys/types.h>

typedef struct process_info {
    const char *curr_file;
    pid_t child_pid;
} pinfo_t;

/**
 * @brief Updates process info
 */
void update_pinfo();

/**
 * @brief Gets process info
 *
 * @return process info
 */
pinfo_t *get_pinfo();

#endif  // INCLUDE_PROCESS_H_
