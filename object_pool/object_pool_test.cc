#include <iostream>
#include "object_pool.h"

using excercise::ObjectPool;

int main() {
  ObjectPool<int> obj_pool(10);
  std::cout << "hello " << std::endl;
  return 0;    
}