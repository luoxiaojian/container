#include "include/vvector.h"

#include <stdio.h>

int main() {
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
  return 0;
}
