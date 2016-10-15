#include "alloc_big.h"

void* alloc_big(size_t size)
{
  void* ptr;
  unsigned char* cp;
  size_t* sp;
  ptr = mmap(NULL, size + META_BIG, PROT_READ | PROT_WRITE,
             MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (ptr != MAP_FAILED)
  {
    cp = ptr;
    set_type(cp, BIG);
    sp = ptr;
    sp++;
    *sp = size;
    return sp + 1;
  }
  else
    return NULL;
}

void free_big(void* ptr, size_t size)
{
  size_t* sp = ptr;
  sp -= 2;
  munmap(ptr, size + META_BIG);
}

void* realloc_big(void* ptr, size_t size, size_t new_size)
{
  size_t* sp = ptr;
  sp--;
  *sp = new_size;
  sp--;
//  ptr = mremap(sp, size + META_BIG, new_size + META_BIG, MREMAP_MAYMOVE);
  
    ptr = mmap(NULL, new_size + META_BIG, PROT_READ | PROT_WRITE,
               MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    memcpy(ptr, sp, size + META_BIG);
    munmap(sp, size + META_BIG);
    
  return ptr;
}
