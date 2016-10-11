#include "size.h"

size_t get_size(void* ptr)
{
  size_t* p = ptr;
  p--;
  return *p;
}
