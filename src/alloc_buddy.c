#include "alloc_buddy.h"

static void* pages[BUDDY_LEVELS] = { NULL };
static void* last_page = NULL;

size_t max_bytes(int level)
{
  int bytes = SIZE_PAGE >> level;
  return bytes - sizeof (size_t);
}

static int needed_level(size_t size)
{
  int level;
  for (level = 0; max_bytes(level + 1) >= size && level < BUDDY_LEVELS - 1;
       level++)
    continue;
  return level;
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

static int get_free(unsigned char* cp)
{
  return get_bit(cp, 6);
}

static int is_empty(void* ptr)
{
  return get_buddy_size(ptr) == 0 && get_free(ptr);
}

static void delete_page(void* ptr)
{
  munmap(ptr, SIZE_PAGE);
  for (int i = 0; i < BUDDY_LEVELS; i++)
  {
    if (pages[i] == ptr)
      pages[i] = NULL;
  }
  if (last_page == ptr)
    last_page = NULL;
}

static void create_page(int level)
{
  void* ptr;
  unsigned char* cp;
  int i;
  if (pages[level] != last_page && last_page)
    ptr = last_page;
  else
  {
    ptr = mmap(NULL, SIZE_PAGE, PROT_READ | PROT_WRITE,
               MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    cp = ptr;
    set_type(cp, BUDDY);
    set_buddy_size(cp, 0);
    set_free(cp, 1);
  }
  for (i = 0; i <= level; i++)
    pages[i] = ptr;
  for (; i < BUDDY_LEVELS; i++)
  {
    if (!pages[i])
      pages[i] = ptr;
  }
  last_page = ptr;
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

static void merge_buddy(void* ptr)
{
  unsigned char* cp = ptr;
  int level = get_buddy_size(cp);
  unsigned char* cpb = change_side(cp, level);
  if (level != 0 && level == get_buddy_size(cpb)
      && get_free(ptr) && get_free(cpb))
  {
    cp = get_left_buddy(cp, level);
    set_buddy_size(cp, level - 1);
    merge_buddy(cp);
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
    else if (side == LEFT && level != 0)
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
  int level = needed_level(size);
  void *ptr;
  if (!pages[level])
    create_page(level);

  ptr = alloc_in_page(pages[level], size, 0, LEFT);
  if (ptr)
    return ptr;
  else
  {
    create_page(level);
    return alloc_buddy(size);
  }
}

void free_buddy(void* ptr, size_t size)
{
  size_t* sp = ptr;
  unsigned char* cp;
  sp--;
  ptr = sp;
  cp = ptr;
  set_free(cp, 1);
  merge_buddy(sp);
  size = size;

  if (is_empty(sp))
    delete_page(sp);
}

void* realloc_buddy(void* ptr, size_t size, size_t new_size)
{
  size_t* sp = ptr;
  void* res;
  unsigned char* cp;
  int level;
  sp--;
  ptr = sp;
  cp = ptr;
  level = get_buddy_size(cp);
  if (size >= new_size)
  {
    set_free(cp, 1);
    res = alloc_in_page(ptr, new_size, level, LEFT);
  }
  else
  {
    size_t *nptr1 = expand_buddy(ptr, new_size, level);
    if (nptr1)
      res = nptr1 + 1;
    else
    {
      void* nptr2 = malloc_no_mutex(new_size);
      if (nptr2)
      {
        sp++;
        memcpy(nptr2, sp, size);
        free_buddy(sp, size);
      }
      res = nptr2;
    }
  }
  return res;
}
