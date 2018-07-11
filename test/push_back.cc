#include "include/gcontainer.h"

#include <stdio.h>

int main() {
  gvector<int> vec;

  for (int i = 0; i < 100; i++) {
    vec.push_back(i * 10);
  }

  for (int i = 0; i < 100; i++) {
    if (vec[i] != i * 10) {
      printf("Error on %d, vec[%d] = %d\n", i, i, vec[i]);
    }
  }

  return 0;
}
