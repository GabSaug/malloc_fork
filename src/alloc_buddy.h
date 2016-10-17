#ifndef ALLOC_BUDDY_H_
# define ALLOC_BUDDY_H_

# define _GNU_SOURCE

# include <sys/mman.h>
# include <string.h>
# include "malloc.h"
# include "malloc_no_mutex.h"
# include "find.h"

enum Side
{
  LEFT,
  RIGHT 
};

size_t max_bytes(int level);
int get_buddy_size(unsigned char* ptr);
void* alloc_buddy(size_t size);
void free_buddy(void* ptr, size_t size);
void* realloc_buddy(void* ptr, size_t size, size_t new_size);

#endif /* !ALLOC_BUDDY_H_ */
