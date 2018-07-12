#include "include/gcontainer.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <utility>
#include <vector>

#define SIZE 1024 * 1024 * 32
#define ITER 32

double get_current_time() {
  timeval t;
  gettimeofday(&t, 0);
  return static_cast<double>(t.tv_sec) +
         static_cast<double>(t.tv_usec) / 1000000.0;
}

int main() {
  for (int i = 0; i < ITER; i++) {
    GVector<double> vec(SIZE);
  }
  double t1 = get_current_time();
  for (int i = 0; i < ITER; i++) {
    GVector<double> vec(SIZE);
  }
  double t2 = get_current_time();

  for (int i = 0; i < ITER; i++) {
    double *buf = reinterpret_cast<double *>(malloc(sizeof(double) * SIZE));
    free(buf);
  }

  double t3 = get_current_time();
  for (int i = 0; i < ITER; i++) {
    double *buf = reinterpret_cast<double *>(malloc(sizeof(double) * SIZE));
    free(buf);
  }
  double t4 = get_current_time();

  for (int i = 0; i < ITER; i++) {
    std::vector<double> vec(SIZE);
  }
  double t5 = get_current_time();
  for (int i = 0; i < ITER; i++) {
    std::vector<double> vec(SIZE);
  }
  double t6 = get_current_time();

  printf("no value:\n");
  printf("GVector: %lf\n", t2 - t1);
  printf("malloc:  %lf\n", t4 - t3);
  printf("vector:  %lf\n", t6 - t5);

  for (int i = 0; i < ITER; i++) {
    GVector<double> vec(SIZE, 3.0);
  }
  t1 = get_current_time();
  for (int i = 0; i < ITER; i++) {
    GVector<double> vec(SIZE, 3.0);
  }
  t2 = get_current_time();

  for (int i = 0; i < ITER; i++) {
    double *buf = reinterpret_cast<double *>(malloc(sizeof(double) * SIZE));
    std::fill_n(buf, SIZE, 3.0);
    free(buf);
  }

  t3 = get_current_time();
  for (int i = 0; i < ITER; i++) {
    double *buf = reinterpret_cast<double *>(malloc(sizeof(double) * SIZE));
    std::fill_n(buf, SIZE, 3.0);
    free(buf);
  }
  t4 = get_current_time();

  for (int i = 0; i < ITER; i++) {
    std::vector<double> vec(SIZE, 3.0);
  }
  t5 = get_current_time();
  for (int i = 0; i < ITER; i++) {
    std::vector<double> vec(SIZE, 3.0);
  }
  t6 = get_current_time();
  printf("value:\n");
  printf("GVector: %lf\n", t2 - t1);
  printf("malloc:  %lf\n", t4 - t3);
  printf("vector:  %lf\n", t6 - t5);

  return 0;
}
