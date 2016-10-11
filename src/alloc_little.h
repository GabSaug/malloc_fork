#ifndef ALLOC_LITTLE_H_
# define ALLOC_LITTLE_H_

# define _DEFAULT_SOURCE

# include <sys/mman.h>
# include "malloc.h"
# include "find.h"

void* alloc_little(size_t size);
void free_little(void* ptr, size_t size);
void* realloc_little(void* ptr, size_t size);

#endif /* !ALLOC_LITTLE_H_ */
