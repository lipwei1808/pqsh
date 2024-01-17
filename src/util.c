#include "sys/time.h"
#include "stdio.h"

double getTime() {
  struct timeval tv;
  if (gettimeofday(&tv, NULL) < 0) {
      printf("Error getting time of day\n");
      return -1;
  }
  return tv.tv_sec;
}