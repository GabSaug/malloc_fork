#ifndef MALLOC_H_
# define MALLOC_H_

# include <stddef.h>
# include <unistd.h>
# include <limits.h>

# define PAGE_SIZE sysconf(_SC_PAGESIZE)
# define MAX_LITTLE_SIZE (64 - sizeof (size_t))
# define MAX_BUDDY_SIZE (PAGE_SIZE - sizeof (size_t))

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
