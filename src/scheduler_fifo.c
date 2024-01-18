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

    Queue waiting = s->waiting;
    Queue running = s->running;
    Process* p = queue_pop(&waiting);
    process_start(p);
    queue_push(&running, p);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
