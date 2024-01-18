/* process.c: PQSH Process */

#include "pqsh/macros.h"
#include "pqsh/process.h"
#include "pqsh/timestamp.h"
#include "pqsh/util.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

/**
 * Create new process structure given command.
 * @param   command     String with command to execute.
 * @return  Pointer to new process structure
 **/
Process *process_create(const char *command) {
    /* TODO: Implement */
    // Allocates space on the heap to fit a Process and returns a pointer 
    // to that memory address.
    Process* process = (Process*) malloc(sizeof (struct Process));
    if (process == NULL) {
        return NULL;
    }
    strncat(process->command, command, BUFSIZ - 1);
    process->pid = -1;
    double time = getTime();
    if (time == -1) {
        free(process);
        printf("Error getting time\n");
        return NULL;
    }
    process->arrival_time = time;
    return process;
}

bool handle_child(Process* p) {
    char* argv[MAX_ARGUMENTS];
    char command[BUFSIZ];
    strcpy(command, p->command);
    int i = 0;
    for (char* token = strtok(command, " "); token; token = strtok(NULL, " ")) {
        argv[i++] = token;
    }
    argv[i] = NULL;

    if (execvp(argv[0], argv) == -1) {
        printf("Failed to execute process: %s\n", p->command);
        return false;
    }
    return true;
}

/**
 * Start process by forking and executing the command.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not starting the process was successful
 **/
bool process_start(Process *p) {
    if (p->pid != -1) {
        printf("Error trying to start process that has already started (pid=%d)\n", p->pid);
        return false;
    }
    pid_t pid = fork();
    switch (pid) {
        // error
        case -1: {
            return false;
        }
        // child process
        case 0: {
            return handle_child(p);
        }
        // parent process
        default: {
            double time = getTime();
            if (time == -1) {
                if (kill(pid, SIGKILL) == -1) {
                    printf("Failed to kill child process!\n");
                }
                return false;
            }
            p->pid = pid;
            p->start_time = time;
            /* TODO: Implement */
            return true;
        }
    }
}

/**
 * Pause process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_pause(Process *p) {
    if (p->pid == -1) {
        printf("Invalid process pid to pause\n");
        return false;
    }

    kill(p->pid, SIGSTOP);
    return true;
}

/**
 * Resume process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_resume(Process *p) {
    if (p->pid == -1) {
        printf("Invalid process pid to resume\n");
        return false;
    }

    kill(p->pid, SIGCONT);
    return true;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
