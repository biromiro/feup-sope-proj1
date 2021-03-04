#include <stdint.h>
#include <sys/stat.h>
#include "args_parser.h"
#include "file_status.h"

int main(int argc, char *argv[], char *envp[]) {
    struct stat status;

    if(argc < 2) return 1;
    
    get_status(argv[1], &status);

    printf("IS DIR? %d\n", is_dir(&status));
    printf("IS LINK? %d\n", is_slink(&status));
    printf("Has access terms: %o\n", get_access_perms(&status));

    return 0;
}
