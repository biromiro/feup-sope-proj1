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
#include <time.h>
#include <unistd.h>

// preparation for execve:
char *cmdline[] = {"sleep", "3", (void *)0};
char *newenviron[] = {"USER=Linus", NULL};

static bool waiting = false;
static pid_t id = -1;
static time_t last_recv;

int get_clean_char() {
    char c = getchar(), tmp;
    while (((tmp = getchar()) != EOF) && (tmp != '\n'))
        ;
    return c;
}

typedef enum {
    SEND,
    RECEIVE,
} pipe_protocol_t;

void exchange_pipe(pipe_protocol_t stat) {
    switch (stat) {
        case SEND: {
        }

        break;

        default:
            break;
    }
}

void sighandler(int signo) {
    if (waiting) {
        printf("Process killed\n");
        exit(1);
    }
    waiting = true;

    printf("%d ; %s ; %d ; %d \n", getpid(), id > 0 ? "random_file" : "child", 5, 4);
    if (getpid() == getpgrp()) {
        last_recv = time(NULL);
        while (time(NULL) <= last_recv) {
        }

        printf("Do you want to exit? (y/Y or n/N)\n");
        char c = get_clean_char();
        printf("CHAR OBTAINED: %c\n", c);
        switch (toupper(c)) {
            case 'Y':
                printf("SENT SIGNAL\n");
                killpg(getpgrp(), SIGINT);
                exit(1);
                break;
            case 'N':
            default:
                killpg(getpgrp(), SIGCONT);
                break;
        }
    } else {
        kill(getpgrp(), SIGUSR1);
    }
}

void cont_processes() {
    if (waiting) {
        printf("Continue process\n");
        waiting = false;
    }
}

void cool() {
    last_recv = time(NULL);
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

    new.sa_handler = cont_processes;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGCONT, &new, &old) == -1)
        perror("sigaction");

    if (sigemptyset(&smask) == -1)
        perror("mask");

    new.sa_handler = cool;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGUSR1, &new, &old) == -1)
        perror("sigaction");
}

int main(int argc, char *argv[], char *envp[]) {
    setup_handler();

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
