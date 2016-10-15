#ifndef ALLOC_BIG_H_
# define ALLOC_BIG_H_

# define _GNU_SOURCE

# define META_BIG (2 * sizeof (size_t))

# include <sys/mman.h>
# include "malloc.h"
# include "find.h"

void* alloc_big(size_t size);
void free_big(void* ptr, size_t size);
void* realloc_big(void* ptr, size_t size, size_t new_size);

#endif /* !ALLOC_BIG_H_ */
