#include "include/gcontainer.h"

#include <stdio.h>

int main() {
  GVector<int> vec;
  vec.Resize(100);

  for (int i = 0; i < 100; i++) {
    vec[i] = i * 10;
  }

  for (int i = 0; i < 100; i++) {
    if (vec[i] != i * 10) {
      printf("Error on %d, vec[%d] = %d\n", i, i, vec[i]);
    }
  }

  return 0;
}
