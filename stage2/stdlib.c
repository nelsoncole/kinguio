#include <stdlib.h>
#include <string.h>

void initialize_heap(unsigned int start)
{
	heap = start;
	
	// clean
	memset((void*)heap,0,DEF_HEAP_LIMIT);
	
}

void *malloc(size_t size)
{
	heap += size;
	return (void*)(heap - size);

}
