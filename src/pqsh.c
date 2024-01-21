/* pqsh.c: Process Queue Shell */

#include "pqsh/macros.h"
#include "pqsh/options.h"
#include "pqsh/scheduler.h"
#include "pqsh/signal.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>
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

int main(int argc, char *argv[]) {
    Scheduler *s = &PQShellScheduler;

    Queue q = {.head = NULL, .tail = NULL, .size = 0};
    s->running = q;
    s->waiting = q;
    s->finished = q;

    s->total_turnaround_time = 0;
    s->total_response_time = 0;

    /* TODO: Parse command line options */
    bool parse_success = my_parse(argc, argv, s);
    if (!parse_success) {
        return 1;
    }

    scheduler_print(s);
    /* TODO: Register signal handlers */
    signal_register(SIGALRM, SA_RESTART, sigalrm_handler);

    /* TODO: Start timer interrupt */
    struct timeval time = {.tv_sec = 0, .tv_usec = s->timeout};
    struct itimerval timer = {.it_interval = time, .it_value = time };
    if (setitimer(ITIMER_REAL, &timer, NULL) < 0) {
        printf("Failed to set timer err=%d\n", errno);
    }

    /* TODO: Process shell comands */
    while (!feof(stdin)) {
        char input[BUFSIZ]  = "";
        char command[BUFSIZ]  = "";
        char argument[BUFSIZ] = "";
        char space[2] = " ";

        printf("\nPQSH> ");

        while (!fgets(input, BUFSIZ, stdin) && !feof(stdin));
        chomp(input);

        // Parse user input into command and arguments
        char* token = strtok(input, " ");
        if (token != NULL) {
            strcat(command, token);
        }
        token = strtok(NULL, " ");
        while (token != NULL) {
            strcat(argument, token);
            token = strtok(NULL, " ");
            if (token != NULL) {
                strcat(argument, space);
            }
        }

        /* TODO: Handle add and status commands */
        if (streq(command, "help")) {
            help();
        } else if (streq(command, "exit") || streq(command, "quit")) {
            break;
        } else if (streq(command, "add")) {
            scheduler_add(s, stdout, argument);
        } else if (streq(command, "status")) {
            scheduler_status(s, stdout, atoi(argument));
        } else if (strlen(command)) {
            printf("Unknown command: %s\n", command);
        }
    }

    scheduler_cleanup(s);
    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */