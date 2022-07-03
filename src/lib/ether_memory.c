#include "ether_defines.h"
#include "ether_private.h"

static unsigned char *_ether_memory_our_pool      = NULL;
static unsigned char *_ether_memory_our_highwater = NULL;
static unsigned int   _ether_memory_memspace      = 0;
static unsigned int   _ether_memory_memused       = 0;
static int            _ether_memory_out_of_memory = 0;     /**< set if the renderer ran out of memory */

int _ether_memory_init(unsigned int maxmem) {
	_ether_memory_our_pool = (unsigned char *)malloc(maxmem);
	if(_ether_memory_our_pool == NULL)
		return -1;

	_ether_memory_memspace = maxmem;
	_ether_memory_our_highwater = _ether_memory_our_pool;
	_ether_memory_memused = 0;

	return 0;
}

void  _ether_memory_quit(void) {
	if (_ether_memory_our_pool)
		free(_ether_memory_our_pool);

	_ether_memory_memused = 0;
}

void  _ether_memory_clear(void) {
	_ether_memory_our_highwater = _ether_memory_our_pool;
	_ether_memory_memused = 0;
}

/* n can be zero (returns pointer to next malloc's return value) */
void * _ether_memory_alloc(int n) {
	void *p = _ether_memory_our_highwater;
	if(_ether_memory_memused + n > _ether_memory_memspace)
	{
		_ether_memory_out_of_memory = 1;
		return NULL;
	}
	_ether_memory_memused += n;
	_ether_memory_our_highwater = &_ether_memory_our_pool[_ether_memory_memused];
	return p;
}

void * _ether_memory_mark(void) {
	return _ether_memory_our_highwater;
}

void  _ether_memory_release(void *ptr) {
	_ether_memory_our_highwater = ptr;
}

