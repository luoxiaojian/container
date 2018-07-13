#include <stdio.h>
#include "include/dcontainer.h"

int main() {
  {
    DArray<double> darr(10, 100.0);
    for (int i = 0; i < 10; ++i) {
      darr[i] += (100.0 * i);
    }

    darr.Dump("./yy");
  }

  {
    DArray<double> da2;
    da2.Load("./yy");
    printf("DArray size = %lu\n", da2.Size());
    for (int i = 0; i < da2.Size(); ++i) {
      printf("DArray[%d] = %lf\n", i, da2[i]);
    }
  }

  return 0;
}
