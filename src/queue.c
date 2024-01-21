/* queue.c: PQSH Queue */

#include "pqsh/macros.h"
#include "pqsh/queue.h"

#include <assert.h>

/**
 * Push process to back of queue.
 * @param q     Pointer to Queue structure.
 **/
void        queue_push(Queue *q, Process *p) {
    /* TODO: Implement */
    if (q->size == 0) {
        q->head = p;
        q->tail = p;
    } else {
        q->tail->next = p;
        q->tail = p;
    }
    (q->size)++;
}

/**
 * Pop process from front of queue.
 * @param q     Pointer to Queue structure.
 * @return  Process from front of queue.
 **/
Process *   queue_pop(Queue *q) {
    /* TODO: Implement */
    if (q->size == 0) {
        return NULL;
    }

    Process* p = q->head;
    if (q->size == 1) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
        p->next = NULL;
        return p;
    }
    q->size = q->size - 1;
    q->head = p->next;
    p->next = NULL;
    return p;
}

/**
 * Remove and return process with specified pid.
 * @param q     Pointer to Queue structure.
 * @param pid   Pid of process to return.
 * @return  Process from Queue with specified pid.
 **/
Process *   queue_remove(Queue *q, pid_t pid) {
    /* TODO: Implement */
    if (q->size == 0) {
        return NULL;
    }

    Process* curr = q->head;
    if (q->size == 1) {
        if (q->head->pid != pid) {
            return NULL;
        }
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
        curr->next = NULL;
        return curr;
    }

    Process* parent = NULL;
    while (curr->pid != pid && curr->next) {
        parent = curr;
        curr = curr->next;
    }

    if (curr->pid == pid) {
        if (parent == NULL) {
            q->head = curr->next;
        } else {
            parent->next = curr->next;
        }
        (q->size)--;
        curr->next = NULL;
        return curr;
    }

    return NULL;
}

/**
 * Dump the contents of the Queue to the specified stream.
 * @param q     Queue structure.
 * @param fs    Output file stream.
 **/
void        queue_dump(Queue *q, FILE *fs) {
    if (q->size == 0) {
        return;
    }

    fprintf(fs, "%6s %-30s %-13s %-13s %-13s\n", 
                "PID", "COMMAND", "ARRIVAL", "START", "END");
    /* TODO: Display information for each item in Queue. */
    Process* cur = q->head;
    while (cur != NULL) {
        fprintf(fs, "%6d %-30s %-13.2f %-13.2f %-13.2f\n",
                cur->pid, cur->command, cur->arrival_time, cur->start_time, cur->end_time);
        cur = cur->next;
    }
}

void queue_cleanup(Queue* q) {
    if (q->size == 0) {
        return;
    }

    Process* cur = q->head;
    while (cur != NULL) {
        free(cur);
        cur = cur->next;
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
