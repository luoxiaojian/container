#include <stdio.h>
#include "include/dcontainer.h"

int main() {
  {
    DVector<double> dvec(3, 100.0);
    dvec[1] += 100.0;
    dvec[2] += 200.0;

    for (int i = 3; i < 5; ++i) {
      dvec.PushBack(100.0 * (i + 1));
    }

    dvec.Resize(7, 600.0);
    dvec[6] += 100.0;

    dvec.Dump("./xx");
  }

  {
    DVector<double> dv2;
    dv2.Load("./xx");
    printf("DVector size = %lu\n", dv2.Size());
    for (int i = 0; i < dv2.Size(); ++i) {
      printf("DVector[%d] = %lf\n", i, dv2[i]);
    }
  }

  return 0;
}
