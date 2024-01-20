/* timestamp.c: PQSH Timestamp */

#include "pqsh/timestamp.h"

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

/**
 * Return current timestamp as a double.
 *
 * Utilizes gettimeofday for precision (accounting for seconds and
 * microseconds) and falls back to time (only account for seconds) if that
 * fails.
 *
 * @return  Double representing the current time.
 **/
double timestamp() {
    /* TODO: Implement */
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0) {
        printf("Error getting time of day\n");
        return -1;
    }
    int microseconds = tv.tv_usec;
    double res = tv.tv_sec + microseconds * pow(10, -6);
    return res;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
