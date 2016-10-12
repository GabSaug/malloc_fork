#include "alloc_buddy.h"

//static void* addr_pages[BUDDY_LEVELS] = { NULL };
static void* page = NULL;

static size_t max_bytes(int level)
{
  int bytes = PAGE_SIZE >> level;
  return bytes - sizeof (size_t);
}

static void set_buddy_size(unsigned char* cp, int level)
{
  unsigned char size = level & 0x3F;
  *cp |= size;
}

static int get_buddy_size(unsigned char* cp)
{
  unsigned char size = *cp;
  int level = size & 0x3F;
  return level;
}

static void* change_side(void* ptr, int level)
{
  size_t size = max_bytes(level) + sizeof (size_t);
  char* cp = ptr;
  char* nul = NULL;
  size_t n = cp - nul;
  n ^= size;
  ptr = nul + n;
  return ptr;
}

static void set_free(unsigned char* cp, int f)
{
  set_bit(cp, 6, f);
}

static void* create_page(void)
{
  void* ptr;
  unsigned char* cp;
  ptr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
             MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  cp = ptr;
  set_type(cp, BUDDY);
  set_buddy_size(cp, 0);
  set_free(cp, 1);
  return ptr;
}

static int get_free(unsigned char* cp)
{
  return get_bit(cp, 6);
}

static void cut_buddy(void* ptr, int level)
{
  set_buddy_size(ptr, level + 1);
  ptr = change_side(ptr, level + 1);
  set_buddy_size(ptr, level + 1);
  set_free(ptr, 1);
}

static void* alloc_in_page(void* ptr, size_t size, int level, enum Side side)
{
  size_t* sp = ptr;
  unsigned char* cp = ptr;
  if (max_bytes(level) < size)
    return NULL;
  if (get_buddy_size(cp) < level)
  {
    void* nptr = alloc_in_page(ptr, size, level + 1, size);
    if (nptr)
      return nptr;
    else if (side == RIGHT)
      return alloc_in_page(change_side(ptr, level), size, level, LEFT);
    else
      return NULL;
  }
  else if (get_buddy_size(cp) == level)
  {
    if (get_free(cp))
    {
      if (max_bytes(level + 1) < size)
      {
        set_free(cp, 0);
        return sp + 1;
      }
      else
      {
        cut_buddy(ptr, level);
        return alloc_in_page(ptr, size, level + 1, RIGHT);
      }
    }
    else if (side == RIGHT)
      return alloc_in_page(change_side(ptr, level), size, level, LEFT);
    else
      return NULL;
  }
  else
    return NULL;
}

void* alloc_buddy(size_t size)
{
  void *ptr;
  if (!page)
    page = create_page();

  ptr = alloc_in_page(page, size, 0, LEFT);

  return ptr;
}

void free_buddy(void* ptr, size_t size)
{

}

void* realloc_buddy(void* ptr, size_t size, size_t new_size)
{

  return ptr;
}
