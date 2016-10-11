#ifndef FIND_H_
# define FIND_H_

# include <stddef.h>
# include "malloc.h"
# include "bit.h"

size_t get_size(void* ptr);
void set_type(unsigned char* cp, enum AllocType type);
enum AllocType get_type(unsigned char* cp);

#endif /* !FIND_H_ */
