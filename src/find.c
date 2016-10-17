#include "find.h"

void* get_page_addr(void* ptr)
{
  char* cp = ptr;
  char* nul = NULL;
  size_t n = cp - nul;
  n &= ~0xFFF;
  ptr = nul + n;
  return ptr;
}

size_t get_size(void* ptr)
{
  size_t* sp;
  void* pa = get_page_addr(ptr);
  unsigned char* cp;
  enum AllocType type;
  cp = pa;
  sp = pa;
  type = get_type(cp);
  if (type == BIG)
    return *(sp + 1);
  else if (type == BUDDY)
  {
    sp = ptr;
    sp--;
    ptr = sp;
    cp = ptr;
    return max_bytes(get_buddy_size(cp));
  }
  else
    return 0;
}
void set_type(unsigned char* cp, enum AllocType type)
{
  if (type == BIG)
  {
    set_bit(cp, 7, 1);
    set_bit(cp, 6, 0);
  }
  if (type == BUDDY)
    set_bit(cp, 7, 0);
  if (type == LITTLE)
  {
    set_bit(cp, 7, 1);
    set_bit(cp, 6, 1);
  }
}

enum AllocType get_type(unsigned char* cp)
{
  cp = get_page_addr(cp);
  if (!get_bit(cp, 7))
    return BUDDY;
  else
  {
    if (get_bit(cp, 6))
      return LITTLE;
    else
      return BIG;
  }
}
