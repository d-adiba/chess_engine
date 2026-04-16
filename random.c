
#include "random.h"


static uint32_t seed = 1804289383;

uint32_t xorshift_random_u32(void)
{
	uint32_t x = seed;
	x ^= x << 13; 
	x ^= x >>17;
	x ^= x << 5;
	seed = x;
	return x; 
}

void xorshift_init(void)
{
	seed = 1804289383; 
}

uint64_t get_random_u64(void)
{
	uint64_t n1, n2, n3, n4; 
	n1 =  (xorshift_random_u32() & 0xFFFF);	
	n2 =  (xorshift_random_u32() & 0xFFFF);
	n3 =  (xorshift_random_u32() & 0xFFFF);
	n4 =  (xorshift_random_u32() & 0xFFFF);
	return n1 | (n2 << 16) | (n3 << 32)  | (n4 << 48);
}


uint64_t get_magic_number(void)
{
	return get_random_u64() & get_random_u64() & get_random_u64(); 
}
