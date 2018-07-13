#include "include/vvector.h"

#include <stdio.h>

int main() {
  {
    VNode begin(100);
    VNode end(110);
    VRange range(begin, end);
    printf("range.size() = %lu\n", range.size());
    VVector<double> rvec(range, 500.0);
    VNode cur;
    double val = 2.0;
    for (cur = begin; cur != end; ++cur) {
      rvec[cur] += val;
      val += 2.0;
    }
    for (cur = begin; cur != end; ++cur) {
      printf(" -> %lf\n", rvec[cur]);
    }
  }
  {
    unsigned id_mask = 1;
    id_mask = (id_mask << 31);
    unsigned a = id_mask - 10u;
    unsigned b = id_mask;
    VNode begin(a);
    VNode end(b);
    VRange range(begin, end);
    printf("range.size() = %lu\n", range.size());
    VVector<double> rvec(range, 500);
    VNode cur;
    double val = 2.0;
    for (cur = begin; cur != end; ++cur) {
      rvec[cur] += val;
      val += 2.0;
    }
    for (cur = begin; cur != end; ++cur) {
      printf(" -> %lf\n", rvec[cur]);
    }
  }
  return 0;
}
