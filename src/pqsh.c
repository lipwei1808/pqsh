/* pqsh.c: Process Queue Shell */

#include "pqsh/macros.h"
#include "pqsh/options.h"
#include "pqsh/scheduler.h"
#include "pqsh/signal.h"

#include <errno.h>
#include <string.h>
#include <sys/time.h>

#include<unistd.h>

/* Global Variables */

Scheduler PQShellScheduler = {
    .policy    = FIFO_POLICY,
    .cores     = 1,
    .timeout   = 250000,
};

/* Help Message */

void help() {
    printf("Commands:\n");
    printf("  add    command    Add command to waiting queue.\n");
    printf("  status [queue]    Display status of specified queue (default is all).\n");
    printf("  help              Display help message.\n");
    printf("  exit|quit         Exit shell.\n");
}


/* Main Execution */
#define TESTING 1
#if TESTING
int main() {
    while (true) {
        char input[20];
        scanf("%s", input);
        printf("Your input: %s\n", input);
        if (input == "bye") {
            return 0;
        }
        Process *p = process_create(input);
        printf("Process created with pid=%d\n", p->pid);
        bool res = process_start(p);
        if (!res) {
            printf("Failed to start process\n");
            return 1;
        }
        printf("Started process with pid: %d\n", p->pid);
        sleep(1);
        res = process_pause(p);
        if (!res) {
            printf("Failed to sleep process with pid: %d\n", p->pid);
        } else {
            printf("Successfully paused process with pid=%d\n", p->pid);
        }
        sleep(5);
        res = process_resume(p);
        if (!res) {
            printf("Failed to resume process with pid: %d\n", p->pid);
        } else {
            printf("Successfully resumed process with pid=%d\n", p->pid);
        }

    }
}

#else
int main(int argc, char *argv[]) {
    Scheduler *s = &PQShellScheduler;

    /* TODO: Parse command line options */

    /* TODO: Register signal handlers */

    /* TODO: Start timer interrupt */

    /* TODO: Process shell comands */
    while (!feof(stdin)) {
        char command[BUFSIZ]  = "";
        char argument[BUFSIZ] = "";

        printf("\nPQSH> ");

        while (!fgets(command, BUFSIZ, stdin) && !feof(stdin));

        chomp(command);
        
        /* TODO: Handle add and status commands */
        if (streq(command, "help")) {
            help();
        } else if (streq(command, "exit") || streq(command, "quit")) {
            break;
        } else if (strlen(command)) {
            printf("Unknown command: %s\n", command);
        }
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */

#endif