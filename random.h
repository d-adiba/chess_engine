#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h> 

void xorshift_init(void);
uint32_t xorshift_random(void);
uint64_t get_random_u64(void);

#endif
