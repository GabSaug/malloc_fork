#include <string.h>
#include <pthread.h>

#include "malloc.h"
#include "malloc_no_mutex.h"

static pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;

__attribute__((__visibility__("default")))
void* malloc(size_t size)
{
  void* ptr;
  pthread_mutex_lock(&mux);
  ptr = malloc_no_mutex(size);
  pthread_mutex_unlock(&mux);
  return ptr;
}

__attribute__((__visibility__("default")))
void free(void* ptr)
{
  pthread_mutex_lock(&mux);
  free_no_mutex(ptr);
  pthread_mutex_unlock(&mux);
}

__attribute__((__visibility__("default")))
void* calloc(size_t number, size_t size)
{
  size_t real_size = number * size;
  pthread_mutex_lock(&mux);
  void* ptr = malloc_no_mutex(real_size);
  if (ptr)
    memset(ptr, 0, real_size);
  pthread_mutex_unlock(&mux);
  return ptr;
}

__attribute__((__visibility__("default")))
void* realloc(void* ptr, size_t new_size)
{
  pthread_mutex_lock(&mux);
  ptr = realloc_no_mutex(ptr, new_size);
  pthread_mutex_unlock(&mux);
  return ptr;
}
