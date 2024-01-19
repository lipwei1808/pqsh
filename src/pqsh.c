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
    .cores     = 2,
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

void child_handler(int v) {
    printf("SIGCHLD Handler invoked %d\n", v);
    scheduler_wait(&PQShellScheduler);
}


/* Main Execution */
#define TESTING 0
#if TESTING
int main() {
    
    Queue running = {.head = NULL, .tail = NULL, .size = 0};
    Queue waiting = {.head = NULL, .tail = NULL, .size = 0};
    Queue finished = {.head = NULL, .tail = NULL, .size = 0};

    PQShellScheduler.running = running,
    PQShellScheduler.waiting = waiting,
    PQShellScheduler.finished = finished,

    PQShellScheduler.total_turnaround_time = 90,
    PQShellScheduler.total_response_time = 0,
    scheduler_print(&PQShellScheduler);
    signal (SIGCHLD, child_handler);
    while (true) {
        char input[20];
        printf(">PQSH ");
        scanf("%s", input);
        printf("Your input: %s\n", input);
        if (strcmp(input, "bye") == 0) {
            return 0;
        }
        scheduler_add(&PQShellScheduler, stdout, input);
        scheduler_fifo(&PQShellScheduler);
    }
}

#else
int main(int argc, char *argv[]) {
    Scheduler *s = &PQShellScheduler;

    Queue running = {.head = NULL, .tail = NULL, .size = 0};
    Queue waiting = {.head = NULL, .tail = NULL, .size = 0};
    Queue finished = {.head = NULL, .tail = NULL, .size = 0};
    s->running = running,
    s->waiting = waiting,
    s->finished = finished,

    s->total_turnaround_time = 0,
    s->total_response_time = 0,

    /* TODO: Parse command line options */

    /* TODO: Register signal handlers */
    signal (SIGCHLD, child_handler);

    /* TODO: Start timer interrupt */

    /* TODO: Process shell comands */
    while (!feof(stdin)) {
        char input[BUFSIZ]  = "";
        char command[BUFSIZ]  = "";
        char argument[BUFSIZ] = "";

        printf("\nPQSH> ");

        while (!fgets(input, BUFSIZ, stdin) && !feof(stdin));
        chomp(input);

        // Parse user input into command and arguments
        char* token = strtok(input, " ");
        strcat(command, token);
        token = strtok(NULL, " ");
        while (token != NULL) {
            strcat(argument, token);
            token = strtok(NULL, " ");
        }

        printf("COMMAND: [%s], ARGUMENT: [%s]\n", command, argument);
        /* TODO: Handle add and status commands */
        if (streq(command, "help")) {
            help();
        } else if (streq(command, "exit") || streq(command, "quit")) {
            break;
        } else if (streq(command, "add")) {
            scheduler_add(s, stdout, argument);
            scheduler_fifo(s); 
        } else if (streq(command, "status")) {
            scheduler_status(s, stdout, 0);
        } else if (strlen(command)) {
            printf("Unknown command: %s\n", command);
        }
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */

#endif