#include "malloc.h"
#include "alloc_little.h"
#include "alloc_buddy.h"
#include "alloc_big.h"

void* malloc_no_mutex(size_t size)
{
  void* ptr;
//  if (size <= MAX_LITTLE_SIZE)
//    ptr = alloc_little(size);
  if (size <= MAX_BUDDY_SIZE)
    ptr = alloc_buddy(size);
  else
    ptr = alloc_big(size);
  return ptr;
}

void free_no_mutex(void* ptr)
{
  if (ptr)
  {
    size_t size = get_size(ptr);
    enum AllocType type = get_type(ptr);
//    if (type == LITTLE)
//      free_little(ptr, size);
    if (type == BUDDY)
      free_buddy(ptr, size);
    else
      free_big(ptr, size);
  }
}

void* realloc_no_mutex(void* ptr, size_t new_size)
{
  if (ptr)
  {
    size_t size = get_size(ptr);
    enum AllocType type = get_type(ptr);
//    if (type == LITTLE)
//      ptr = realloc_little(ptr, size, new_size);
    if (type == BUDDY)
      ptr = realloc_buddy(ptr, size, new_size);
    else
      ptr = realloc_big(ptr, size, new_size);
    return ptr;  
  }
  else
    return malloc_no_mutex(new_size);
}
