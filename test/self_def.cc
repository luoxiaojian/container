#include <stdio.h>

#include "include/gcontainer.h"

class X {
 public:
  X() {
    printf("default construct...\n");
    val = 0;
  }
  X(int v) : val(v) {
    printf("construct-%d...\n", v);
    val = v;
  }
  X(const X& x) : val(x.val) { printf("copy construct-%d...\n", x.val); }
  ~X() { printf("deconstruct-%d...\n", val); }
  int val;
};

int main() {
  gvector<X> vec(2);
  printf("========================================\n");
  vec.Resize(3, X(1));
  printf("========================================\n");
  vec.Resize(4, X(2));
  printf("========================================\n");
  vec.PushBack(X(3));
  printf("========================================\n");
  return 0;
}
