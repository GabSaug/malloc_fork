# 0 "bit.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "bit.c"
# 1 "bit.h" 1



void set_bit(unsigned char* cp, int n, int b);
int get_bit(unsigned char* cp, int n);
# 2 "bit.c" 2

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
