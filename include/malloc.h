#ifndef MALLOC_H_
# define MALLOC_H_

# include <stddef.h>
# include <unistd.h>
# include <limits.h>

# define POW2(n) (0x1 << (n))

# define PAGE_SIZE sysconf(_SC_PAGESIZE)

# define BUDDY_LEVELS 7 
# define MAX_BUDDY_SIZE (PAGE_SIZE - sizeof (size_t) - 1)

# define MAX_LITTLE_SIZE (PAGE_SIZE / POW2(BUDDY_LEVELS - 1) - sizeof (size_t) - 1)

enum AllocType
{
  LITTLE,
  BUDDY,
  BIG
};

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);

#endif /* MALLOC_H_ */
