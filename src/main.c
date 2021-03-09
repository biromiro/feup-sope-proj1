#include "args_parser.h"
#include "dirs.h"
#include "file_status.h"

int main(int argc, char* argv[], char* envp[]) {
    struct stat status;

    if (argc < 2) return 1;

    get_status(argv[1], &status);

    if (is_dir(&status)) recursive_change_mod(argv[1]);

    return 0;
}
