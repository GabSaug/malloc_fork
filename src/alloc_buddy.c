#include "alloc_buddy.h"

//static void* addr_pages[BUDDY_LEVELS] = { NULL };
static void* page = NULL;

size_t max_bytes(int level)
{
  int bytes = PAGE_SIZE >> level;
  return bytes - sizeof (size_t);
}

static void set_buddy_size(unsigned char* cp, int level)
{
  unsigned char mask = 0xC0;
  unsigned char size = level & 0x3F;
  *cp &= mask;
  *cp |= size;
}

int get_buddy_size(unsigned char* cp)
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

static void* get_left_buddy(void* ptr, int level)
{
  void* ptr2 = change_side(ptr, level);
  return (ptr < ptr2) ? ptr : ptr2;
}

static int is_left_buddy(void* ptr, int level)
{
  return ptr == get_left_buddy(ptr, level);
}

static void cut_buddy(void* ptr, int level)
{
  set_buddy_size(ptr, level + 1);
  ptr = change_side(ptr, level + 1);
  set_buddy_size(ptr, level + 1);
  set_free(ptr, 1);
}

static void merge_buddy(void* ptr, int level)
{
  if (level > 0 && get_free(ptr) && get_free(change_side(ptr, level)))
  {
    unsigned char* cp = get_left_buddy(ptr, level);
    set_buddy_size(cp, level - 1);
    merge_buddy(cp, level - 1);
  }
}

static void* alloc_in_page(void* ptr, size_t size, int level, enum Side side)
{
  size_t* sp = ptr;
  unsigned char* cp = ptr;
  if (max_bytes(level) < size)
    return NULL;
  else if (get_buddy_size(cp) > level)
  {
    void* nptr = alloc_in_page(ptr, size, level + 1, LEFT);
    if (nptr)
      return nptr;
    else if (side == LEFT && level != 0)
      return alloc_in_page(change_side(ptr, level), size, level, RIGHT);
    else
      return NULL;
  }
  else if (get_buddy_size(cp) == level)
  {
    if (get_free(cp))
    {
      if (max_bytes(level + 1) < size || level + 1 == BUDDY_LEVELS)
      {
        set_free(cp, 0);
        return sp + 1;
      }
      else
      {
        cut_buddy(ptr, level);
        return alloc_in_page(ptr, size, level + 1, LEFT);
      }
    }
    else if (side == LEFT)
      return alloc_in_page(change_side(ptr, level), size, level, RIGHT);
    else
      return NULL;
  }
  else
    return NULL;
}

static void* expand_buddy(void* ptr, size_t size, int level)
{
  if (max_bytes(level) >= size)
    return ptr;
  else if (level == 0)
    return NULL;
  else if (is_left_buddy(ptr, level) && get_free(change_side(ptr, level)))
  {
    void* ptr2;
    set_buddy_size(ptr, level - 1);
    ptr2 = expand_buddy(ptr, size, level - 1);
    if (ptr2)
      return ptr2;
    else
    {
      set_buddy_size(ptr, level);
      return NULL;
    }
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
  if (ptr)
    return ptr;
  else
  {
    page = NULL;
    return alloc_buddy(size);
  }
}

void free_buddy(void* ptr, size_t size)
{
  size_t* sp = ptr;
  unsigned char* cp;
  int level;
  sp--;
  ptr = sp;
  cp = ptr;
  level = get_buddy_size(cp);
  set_free(cp, 1);
  merge_buddy(sp, level);
  size = size;
}

void* realloc_buddy(void* ptr, size_t size, size_t new_size)
{
  size_t* sp = ptr;
  unsigned char* cp;
  int level;
  sp--;
  ptr = sp;
  cp = ptr;
  level = get_buddy_size(cp);
  if (size >= new_size)
  {
    set_free(cp, 1);
    return alloc_in_page(ptr, new_size, level, LEFT);
  }
  else
  {
    ptr = expand_buddy(ptr, new_size,level);
    if (ptr)
      return ptr;
    else
    {
      void* nptr = malloc(new_size);
      if (nptr)
      {
        memcpy(nptr, ptr, size);
        free_buddy(ptr, size);
      }
      return nptr;
    }
  }
}
