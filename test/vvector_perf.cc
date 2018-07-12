#include "include/vvector.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define SIZE (1024 * 1024 * 128)
#define ITER (8)

double get_current_time() {
  timeval t;
  gettimeofday(&t, 0);
  return static_cast<double>(t.tv_sec) +
         static_cast<double>(t.tv_usec) / 1000000.0;
}

int main() {
  VNode *loc = reinterpret_cast<VNode *>(malloc(sizeof(VNode) * SIZE));
  for (size_t i = 0; i < SIZE; i++) {
    loc[i].SetValue(rand() % SIZE + SIZE);
  }

  VNode begin(SIZE);
  VNode end(2 * SIZE);
  VRange range(begin, end);
  printf("range.size() = %lu\n", range.size());

  {
    VVector<double> rvec(range, 500.0);
    printf("original vector:\n");
    for (int iter = 0; iter < ITER; ++iter) {
      double val = 0.0;
      for (size_t i = 0; i < SIZE; ++i) {
        rvec[loc[i]] += val;
        val += 1.0;
      }
    }

    double t1 = get_current_time();
    for (int iter = 0; iter < ITER; ++iter) {
      double val = 0.0;
      for (size_t i = 0; i < SIZE; ++i) {
        rvec[loc[i]] += val;
        val += 1.0;
      }
    }
    double t2 = get_current_time();

    double sum1 = 0.0, sum2 = 0.0;
    for (int iter = 0; iter < ITER; ++iter) {
      for (size_t i = 0; i < SIZE; ++i) {
        sum1 = (sum1 + rvec[loc[i]]) / 2.0;
      }
    }

    double t3 = get_current_time();
    for (int iter = 0; iter < ITER; ++iter) {
      for (size_t i = 0; i < SIZE; ++i) {
        sum2 = (sum2 + rvec[loc[i]]) / 2.0;
      }
    }
    double t4 = get_current_time();

    printf("write: %lf\n", t2 - t1);
    printf("read: %lf\n", t4 - t3);
    printf("%lf, %lf\n", sum1, sum2);
  }

  {
    NVVector<double> rvec(range, 500.0);
    printf("new vector:\n");
    for (int iter = 0; iter < ITER; ++iter) {
      double val = 0.0;
      for (size_t i = 0; i < SIZE; ++i) {
        rvec[loc[i]] += val;
        val += 1.0;
      }
    }

    double t1 = get_current_time();
    for (int iter = 0; iter < ITER; ++iter) {
      double val = 0.0;
      for (size_t i = 0; i < SIZE; ++i) {
        rvec[loc[i]] += val;
        val += 1.0;
      }
    }
    double t2 = get_current_time();

    double sum1 = 0.0, sum2 = 0.0;
    for (int iter = 0; iter < ITER; ++iter) {
      for (size_t i = 0; i < SIZE; ++i) {
        sum1 = (sum1 + rvec[loc[i]]) / 2.0;
      }
    }

    double t3 = get_current_time();
    for (int iter = 0; iter < ITER; ++iter) {
      for (size_t i = 0; i < SIZE; ++i) {
        sum2 = (sum2 + rvec[loc[i]]) / 2.0;
      }
    }
    double t4 = get_current_time();

    printf("write: %lf\n", t2 - t1);
    printf("read: %lf\n", t4 - t3);
    printf("%lf, %lf\n", sum1, sum2);
  }

  return 0;
}
