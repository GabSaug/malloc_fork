#ifndef ALLOC_BUDDY_H_
# define ALLOC_BUDDY_H_

# define _DEFAULT_SOURCE

# include <sys/mman.h>
# include "malloc.h"
# include "find.h"

void* alloc_buddy(size_t size);
void free_buddy(void* ptr, size_t size);
void* realloc_buddy(void* ptr, size_t size, size_t new_size);

#endif /* !ALLOC_BUDDY_H_ */
