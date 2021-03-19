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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

// preparation for execve:
char* cmdline[] = {"sleep", "3", (void*)0};
char* newenviron[] = {"USER=Linus", NULL};

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

struct sigaction sig_def_term;

void log_term(int signo) {
    printf("PROCESS TERMINATED\n");

    signal(signo, SIG_DFL);
    raise(signo);

    // signal(signo, log_term);
}
void sighandler(int signo) {
    if (waiting) {
        printf("Process killed\n");
        exit(1);
    }
    waiting = true;

    printf("%d ; %s ; %d ; %d \n", getpid(), id > 0 ? "random_file" : "child",
           5, 4);
    if (getpid() == getpgrp()) {
        last_recv = time(NULL);
        while (time(NULL) <= last_recv) {
        }

        printf("Do you want to exit? (y/Y to exit or other to continue)\n");
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
        kill(getpgrp(), SIGCHLD);
    }
}

void cont_processes() {
    if (waiting) {
        printf("Continue process\n");
        waiting = false;
    }
}

void cool() {
    int w_status;
    pid_t pid;
    pid = wait(&w_status);
    printf("instant: %ld\n", clock());

    switch (pid) {
        case 0:
            printf("what?\n");
            perror("cool");
            break;
        case -1:
            printf("apperanlty error\n");
            perror("cool");
            break;
        default:
            printf(
                "info on died child: normal exit? %d, exit status: %d, term by "
                "signal? %d, if so what? %d, was core dump? %d --------------------------------------------------------------\n",
                WIFEXITED(w_status), WEXITSTATUS(w_status),
                WIFSIGNALED(w_status), WTERMSIG(w_status), WCOREDUMP(w_status));
            break;
    }

    last_recv = time(NULL);
    printf("DIED!\n");
}

void setup_handler() {
    struct sigaction new, old;
    sigset_t smask;

    if (sigemptyset(&smask) == -1) perror("mask");

    new.sa_handler = sighandler;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGINT, &new, &old) == -1) perror("sigaction");

    if (sigemptyset(&smask) == -1) perror("mask");

    new.sa_handler = cont_processes;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGCONT, &new, &old) == -1) perror("sigaction");

    if (sigemptyset(&smask) == -1) perror("mask");

    new.sa_handler = cool;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGCHLD, &new, &old) == -1) perror("sigaction");

    if (sigemptyset(&smask) == -1) perror("mask");

    new.sa_handler = log_term;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGTERM, &new, &sig_def_term) == -1) perror("sigaction");
}

int main(int argc, char* argv[], char* envp[]) {
    setup_handler();

    setbuf(stdout, NULL);  // to make sure printf() have no buffer so, nodelay!

    printf("\nPID: %d", getpid());

    id = fork();
    switch (id) {
        case -1:
            perror("fork()");
            exit(1);
        case 0:  // child
            printf("\nPid: %d. Group=%d\n", getpid(), getpgrp());
            int fd = open("testsig", O_EXCL);
            if (fd < 0) {
                perror("open");
                exit(3);
            }
            while (waiting) {
            }
            //raise(SIGTERM);
            char test[1];
            raise(SIGTERM);
            exit(2);
            sleep(1);
            return fexecve(fd, cmdline, newenviron);

        default: {
            id = fork();
            if(id == 0) {
                exit(1);
            } else {
                int status;
                for(uint32_t i = 0; i < UINT16_MAX; i++) printf("test\n");
                while (wait(&status) > 0) {
                    printf("instant %ld, PID:  %d Status: %d\n", clock(), getpid(), WEXITSTATUS(status));
                }
                
            }
            
        }
    }



    return 0;
}
