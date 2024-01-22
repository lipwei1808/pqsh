/* scheduler_rdrn.c: PQSH Round Robin Scheduler */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"

#include <assert.h>

/**
 * Schedule next process using round robin policy:
 *
 *  1. If there no waiting processes, then do nothing.
 *
 *  2. Move one process from front of running queue and place in back of
 *  waiting queue.
 *
 *  3. Move one process from front of waiting queue and place in back of
 *  running queue.
 *
 * @param   s	    Scheduler structure
 **/
void scheduler_rdrn(Scheduler *s) {
    /* TODO: Implement Round Robin Policy */
    if (s->waiting.size == 0) {
        return;
    }

    Process* toSchedule = queue_pop(&s->waiting);
    if (s->running.size >= s->cores) {
        Process* popped = queue_pop(&s->running);
        if (popped == NULL) {
            printf("Error removing process from queue in RDRN\n");
        }
        process_pause(popped);
        queue_push(&s->waiting, popped);
    }

    if (toSchedule->pid == 0) {
        process_start(toSchedule);
        s->total_response_time += (toSchedule->start_time - toSchedule->arrival_time);
        s->total_started++;
    } else {
        process_resume(toSchedule);
    }
    queue_push(&s->running, toSchedule);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
