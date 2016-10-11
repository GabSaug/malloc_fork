#include "find.h"

size_t get_size(void* ptr)
{
  size_t* sp = ptr;
  unsigned char* cp;
  enum AllocType type;
  sp--;
  ptr = sp;
  cp = ptr;
  type = get_type(cp);
  if (type == BIG)
    return *(sp - 1);
  else if (type == BUDDY)
    return 0;
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
