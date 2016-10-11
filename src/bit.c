#include "bit.h"

void set_bit(unsigned char* cp, int n, int b)
{
  unsigned char mask;
  if (b)
  {
    mask = 0x01 << n;
    *cp |= mask;
  }
  else
  {
    mask = 0x01 << n;
    *cp &= ~mask;
  }
}

int get_bit(unsigned char* cp, int n)
{
  unsigned char mask;
  mask = 0x01 << n;
  return *(cp) & mask;
}
