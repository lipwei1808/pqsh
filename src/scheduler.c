/* scheduler.c: PQSH Scheduler */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"
#include "pqsh/timestamp.h"

#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

/**
 * Add new command to waiting queue.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   command Command string for new Process.
 **/
void scheduler_add(Scheduler *s, FILE *fs, const char *command) {
    /* TODO: Implement */
    Process* p = process_create(command);
    if (p == NULL) {
        fprintf(fs, "Failed to create process, command \"%s\" not added to scheduler\n", command);
        return;
    }
    queue_push(&s->waiting, p);
    fprintf(fs, "Added process \"%s\" to waiting queue.\n", command);
}

/**
 * Display status of queues in Scheduler.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, FILE *fs, int queue) {
    fprintf(fs, "Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %05.2lf, Response = %05.2lf\n",
            s->running.size, s->waiting.size, s->finished.size, s->total_turnaround_time, s-> total_response_time);
    /* TODO: Complement implementation. */
}

/**
 * Schedule next process using appropriate policy.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_next(Scheduler *s) {
    /* TODO: Dispatch to appropriate scheduler function. */
    switch (s->policy) {
        case FIFO_POLICY: {
            scheduler_fifo(s);
        }
        case RDRN_POLICY: {
            scheduler_rdrn(s);
        }
        default: {
            printf("Invalid policy\n");
        }
    }
}

/**
 * Wait for any children and remove from queues and update metrics.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_wait(Scheduler *s) {
    /* TODO: Wait for any children without blocking:
     *
     *  - Remove process from queues.
     *  - Update Process metrics.
     *  - Update Scheduler metrics.
     **/
    pid_t pid;
    while (pid = waitpid(-1, NULL, NULL) > 0) {
       Process *found = queue_remove(&s->running, pid);
       queue_push(&s->finished, found);
    }
}

void scheduler_print(Scheduler *s) {
    printf("======Scheduler Settings: %p======\n", s);
    printf("Policy: %s\n", s->policy);
    printf("Cores: %d\n", s->cores);
    printf("Timeout: %d\n", s->timeout);

    printf("Running: %d\n", s->running.size);
    printf("Waiting: %d\n", s->waiting.size);
    printf("Finished: %d\n", s->finished.size);

    printf("%d\n", s->total_response_time);
    printf("Turnaround time: %d\n", s->total_turnaround_time);
    printf("Response time: %d\n", s->total_response_time);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
