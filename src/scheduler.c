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
    if (s == NULL || command == NULL || fs == NULL) {
        return;
    }
    /* TODO: Implement */
    Process* p = process_create(command);
    if (p == NULL) {
        fprintf(fs, "Failed to create process, command \"%s\" not added to scheduler\n", command);
        return;
    }
    queue_push(&s->waiting, p);
}

/**
 * Display status of queues in Scheduler.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, FILE *fs, int queue) {
    fprintf(fs, "Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %05.2lf, Response = %05.2lf\n",
            s->running.size, s->waiting.size, s->finished.size, s->total_turnaround_time / s->finished.size, s->total_response_time / s->total_started);
    /* TODO: Complement implementation. */
    switch (queue) {
        case RUNNING: {
            printf("\nRunning Queue:\n");
            queue_dump(&s->running, fs);
            break;
        }
        case WAITING: {
            printf("\nWaiting Queue:\n");
            queue_dump(&s->waiting, fs);
            break;
        }
        case FINISHED: {
            printf("\nFinished Queue:\n");
            queue_dump(&s->finished, fs);
            break;
        }
        default: {
            if (s->running.size) {
                printf("\nRunning Queue:\n");
                queue_dump(&s->running, fs);
            }
            if (s->waiting.size) {
                printf("\nWaiting Queue:\n");
                queue_dump(&s->waiting, fs);
            }
            if (s->finished.size) {
                printf("\nFinished Queue:\n");
                queue_dump(&s->finished, fs);
            }
        }
    }
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
            break;
        }
        case RDRN_POLICY: {
            scheduler_rdrn(s);
            break;
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
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        Process *found = queue_remove(&s->running, pid);
        if (!found) {
            printf("Unable to remove process (%d) from running queue\n", pid);
            return;
        }
        double time = timestamp();
        found->end_time = time;
        s->total_turnaround_time += (time - found->arrival_time);
        queue_push(&s->finished, found);
    }
}

void scheduler_cleanup(Scheduler* s) {
    queue_cleanup(&s->waiting);
    queue_cleanup(&s->running);
    queue_cleanup(&s->finished);
}

void scheduler_print(Scheduler *s) {
    printf("======Scheduler Settings: %p======\n", s);
    printf("Policy: %u\n", s->policy);
    printf("Cores: %zu\n", s->cores);
    printf("Timeout: %ld\n", s->timeout);

    printf("Running: %zu\n", s->running.size);
    printf("Waiting: %zu\n", s->waiting.size);
    printf("Finished: %zu\n", s->finished.size);

    printf("Turnaround time: %f\n", s->total_turnaround_time);
    printf("Response time: %f\n", s->total_response_time);
    printf("========================\n");
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
