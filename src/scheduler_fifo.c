/* scheduler_fifo.c: PQSH FIFO Scheduler */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"

#include <assert.h>

/**
 * Schedule next process using fifo policy:
 *
 *  As long as we have less running processes than our number of CPUS and
 *  there are waiting processes, then we should move a process from the
 *  waiting queue to the running queue.
 *
 * @param   s	    Scheduler structure
 */
void scheduler_fifo(Scheduler *s) {
    /* TODO: Implement FIFO Policy */
    if (s->running.size >= s->cores) {
        return;
    }

    if (s->waiting.size <= 0) {
        return;
    }

    Process* p = queue_pop(&s->waiting);
    if (p == NULL) {
        return;
    }
    bool res = process_start(p);
    if (!res) {
        printf("Erorr starting child process\n");
        return;
    }
    s->total_response_time += (p->start_time - p->arrival_time);
    s->total_started++;
    queue_push(&s->running, p);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
