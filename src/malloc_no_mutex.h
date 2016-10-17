#ifndef MALLOC_NO_MUTEX_H_
# define MALLOC_NO_MUTEX_H_

void* malloc_no_mutex(size_t size);
void free_no_mutex(void* ptr);
void* realloc_no_mutex(void* ptr, size_t new_size);

#endif /* !MALLOC_NO_MUTEX_H_ */
