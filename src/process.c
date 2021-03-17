#include "../include/process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static pinfo_t pinfo;

void setup_pinfo() {
    pinfo.curr_file = (char*)malloc(2);
    pinfo.curr_file = NULL;
    pinfo.child_pid = -1;
    pinfo.files_found = 0;
    pinfo.files_changed = 0;
}

void update_file_pinfo(const char* path) {
    printf("PINFO: %ld, '%s'\n", (u_int64_t)pinfo.curr_file, pinfo.curr_file);
    pinfo.curr_file = (char*)realloc(pinfo.curr_file, strlen(path) + 1);
    snprintf(pinfo.curr_file, strlen(path) + 1, "%s", path);
}

void update_pid_pinfo(pid_t pid) {
    pinfo.child_pid = pid;
}

void update_file_status_pinfo(bool changed) {
    pinfo.files_found++;
    if (changed) {
        pinfo.files_changed++;
    }
}

pinfo_t* get_pinfo() {
    return &pinfo;
}

int get_super_process() {
    return getpgrp();
}

bool is_root_process() {
    return getpid() == get_super_process();
}