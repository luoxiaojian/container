#include "include/gcontainer.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define SIZE 1024 * 1024 * 128
#define ITER 32

double get_current_time() {
  timeval t;
  gettimeofday(&t, 0);
  return static_cast<double>(t.tv_sec) +
         static_cast<double>(t.tv_usec) / 1000000.0;
}

int main() {
  for (int i = 0; i < ITER; i++) {
    gvector<double> vec(SIZE);
    vec[0] = 0.0;
    vec[SIZE - 1] = 1.0;
  }
  double t1 = get_current_time();
  for (int i = 0; i < ITER; i++) {
    gvector<double> vec(SIZE);
    vec[0] = 0.0;
    vec[SIZE - 1] = 1.0;
  }
  double t2 = get_current_time();

  for (int i = 0; i < ITER; i++) {
    double *buf = reinterpret_cast<double *>(malloc(sizeof(double) * SIZE));
    buf[0] = 0.0;
    buf[SIZE - 1] = 1.0;
    free(buf);
  }

  double t3 = get_current_time();
  for (int i = 0; i < ITER; i++) {
    double *buf = reinterpret_cast<double *>(malloc(sizeof(double) * SIZE));
    buf[0] = 0.0;
    buf[SIZE - 1] = 1.0;
    free(buf);
  }
  double t4 = get_current_time();

  printf("%lf / %lf = %lf\n", t2 - t1, t4 - t3, (t2 - t1) / (t4 - t3));
  return 0;
}
