#include <stdio.h>
#include <string>

#include "include/gcontainer.h"

int main() {
gvector<std::string> vec;
vec.resize(100);
for(int i = 0; i < 100; i++) {
vec[i] = "string-" + std::to_string(i);
}
for(int i = 0; i < 10; i++) {
printf("%s\n", vec[i].c_str());
}
return 0;
}
