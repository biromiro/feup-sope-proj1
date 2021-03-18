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
#include <errno.h>
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

struct sigaction sig_def_term;

void log_term(int signo) {
    printf("PROCESS TERMINATED\n");

    signal(signo, SIG_DFL);
    raise(signo);

    signal(signo, log_term);
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

    if (sigaction(SIGCHLD, &new, &old) == -1)
        perror("sigaction");

    if (sigemptyset(&smask) == -1)
        perror("mask");

    new.sa_handler = log_term;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGTERM, &new, &sig_def_term) == -1)
        perror("sigaction");
}

int int_to_string(int num, char * string, int buffer_size) {
    int cur = 0;

    int aux = num;

    while(aux > 0) {
        cur++;
        aux/= 10;
    }

    if(cur >= buffer_size) cur = buffer_size - 1;
    string[cur] = '\0';
    int size = cur;
    cur--;

    while(num > 0 && cur >= 0) {
        string[cur] = num % 10 + '0'; 
        num /= 10;

        cur--;
    }

    string[cur] = '\0';
    return size;
}


int main(int argc, char *argv[], char *envp[]) {
    char out[255] = "";

    int size = int_to_string(2, out, 255);

    printf("%s\n", out);
    int pipes[2];

    pipe(pipes);
    fcntl(pipes[0], F_SETFL, O_NONBLOCK);
    printf("%d size\n",size);

    write(pipes[1], out, size + 1);
    char get[255];
    char* cur = get;
    int err = 0, signo = 0;

    while (true) {
        err = read(pipes[0], cur, 1);

        if (err == -1) {
            if (errno == EINTR) continue;

            perror("read pipe");
            return 1;
        }

        if (*cur == 0) break;
        cur++;
    }

    signo = atoi(out);

    printf("%d-\n",signo);

    err = read(pipes[0], cur, 1);

    return 0;
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
            sleep(1);
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
