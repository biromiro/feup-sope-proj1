#include "args_parser.h"
#include "dirs.h"
#include "file_status.h"

int main(int argc, char* argv[], char* envp[]) {
    struct stat status;

    if (argc < 2) return 1;

    get_status(argv[1], &status);

    if (is_dir(&status)) open_dir(argv[1], 0);

    return 0;
}
