#include "include/rvector.h"

#include <stdio.h>

int main() {
  Handle<unsigned> begin(100);
  Handle<unsigned> end(110);
  Range<unsigned> range(begin, end);
  printf("range.size() = %lu\n", range.size());
  rvector<double, unsigned> rvec(range, 500.0);
  Handle<unsigned> cur;
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
