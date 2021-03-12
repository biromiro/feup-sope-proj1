/*
*	EXEC: 1st example of typical usage!
*
* Note:	execve is the syscal!
*	execv, execvp, execvpe, execl, execlp, execle are library functions.
*
* Exercise:
* 	change this code to show that USER was changed after the exec.
*/
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// preparation for execve:
char *cmdline[] = {"sleep", "3", (void *)0};
char *newenviron[] = {"USER=Linus", NULL};

static bool waiting = false;

void clear_buffer() {
    char c;
    while (((c = getchar()) != EOF) && (c != '\n'))
        printf("CHAR: %d\n\n", c);
}

void sighandler(int signo) {
    waiting = true;

    printf("%d ; %s ; %d ; %d \n", getpid(), "random_file", 5, 4);
    if (getpid() == getpgrp()) {
        printf("Do you want to exit? (y/Y or n/N)\n");
        char c = getchar();
        clear_buffer(stdin);
        printf("CHAR OBTAINED: %c\n", c);
        switch (toupper(c)) {
            case 'Y':
                printf("SENT SIGNAL\n");
                killpg(getpgrp(), SIGUSR1);
                exit(1);
                break;
            case 'N':
            default:
                killpg(getpgrp(), SIGUSR2);
                break;
        }
    }
}

void confirm_kill(int signo) {
    if (waiting) {
        printf("Process killed\n");
        exit(1);
    }
}

void dummy() {
    if (waiting) {
        printf("Continue process\n");
        waiting = false;
    }
}

void setup_handler() {
    struct sigaction new, old;
    sigset_t smask;

    if (sigemptyset(&smask) == -1)
        perror("mask");

    new.sa_handler = sighandler;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGINT, &new, &old) == -1)
        perror("sigaction");

    if (sigemptyset(&smask) == -1)
        perror("mask");

    new.sa_handler = confirm_kill;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGUSR1, &new, &old) == -1)
        perror("sigaction");

    if (sigemptyset(&smask) == -1)
        perror("mask");

    new.sa_handler = dummy;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGUSR2, &new, &old) == -1)
        perror("sigaction");
}

int main(int argc, char *argv[], char *envp[]) {
    setup_handler();

    pid_t id;
    setbuf(stdout, NULL);  // to make sure printf() have no buffer so, nodelay!

    printf("\nPID: %d", getpid());

    id = fork();
    switch (id) {
        case -1:
            perror("fork()");
            exit(1);
        case 0:  // child
            printf("\nPid: %d. Group=%d", getpid(), getpgrp());
            int fd = open("testsig", O_EXCL);
            if (fd < 0) {
                perror("open");
                exit(3);
            }
            sleep(3);
            while (waiting) {
            }

            return fexecve(fd, cmdline, newenviron);

        default: {
            int status;
            while (1) {
                if (waiting)
                    continue;
                if (wait(&status) > 0)
                    break;
            }
            printf("PID: %d Status: %d\n", getpid(), WEXITSTATUS(status));
        }
    }
    return 0;
}
