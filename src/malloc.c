#include <string.h>

#include "malloc.h"
#include "alloc_little.h"
#include "alloc_buddy.h"
#include "alloc_big.h"

__attribute__((__visibility__("default")))
void* malloc(size_t size)
{
  void* ptr;
  
  if (size <= MAX_LITTLE_SIZE && 0)
    ptr = alloc_buddy(size);
//    ptr = alloc_little(size);
  else if (size <= MAX_BUDDY_SIZE && 0)
    ptr = alloc_buddy(size);
  else
    ptr = alloc_big(size);
  return ptr;
}

__attribute__((__visibility__("default")))
void free(void* ptr)
{
  if (ptr)
  {
    size_t size = get_size(ptr);
    enum AllocType type = get_type(ptr);
    if (type == LITTLE && 0)
      free_buddy(ptr, size);
//      free_little(ptr, size);
    else if (type == BUDDY && 0)
      free_buddy(ptr, size);
    else
    free_big(ptr, size);
  }
}

__attribute__((__visibility__("default")))
void* calloc(size_t number, size_t size)
{
  size_t real_size = number * size;
  void* ptr = malloc(real_size);
  if (ptr)
    memset(ptr, 0, real_size);
  return ptr;
}

__attribute__((__visibility__("default")))
void* realloc(void* ptr, size_t new_size)
{
  if (ptr)
  {
    size_t size = get_size(ptr);
    enum AllocType type = get_type(ptr);
    if (type == LITTLE && 0)
      ptr = realloc_buddy(ptr, size, new_size);
//     ptr = realloc_little(ptr, size, new_size);
    else if (type == BUDDY && 0)
      ptr = realloc_buddy(ptr, size, new_size);
    else
      ptr = realloc_big(ptr, size, new_size);
    return ptr;  
  }
  else
    return malloc(new_size);
}
