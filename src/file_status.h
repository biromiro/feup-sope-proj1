#include <sys/stat.h>
#include <stdbool.h>

int get_status(const char* pathname, struct stat* status);
bool is_dir(const struct stat* status);
bool is_slink(const struct stat* status);
int get_access_perms(const struct stat* status);