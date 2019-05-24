//
// Created by mati on 08.05.19.
//

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "memory/usermem.h"


void free(void *pv)
{
#if PROJECT_CONFIG_MEM_LEAKS_CHECKS == 1
    uint32_t caller = (uint32_t)__builtin_return_address (0);
  memleaks_log_free((uint32_t)pv,caller);
#endif
    return userfree(pv);
}

void *malloc(size_t xWantedSize)
{
    void * ptr = usermalloc(xWantedSize);

#if PROJECT_CONFIG_MEM_LEAKS_CHECKS == 1
    uint32_t caller = (uint32_t)__builtin_return_address (0);
  memleaks_log_malloc((uint32_t)ptr,(uint32_t)caller,xWantedSize);
#endif

    return ptr;
}

void* _malloc_r (struct _reent *r, size_t sz)
{
    void * ptr = usermalloc(sz);

#if PROJECT_CONFIG_MEM_LEAKS_CHECKS == 1
    uint32_t caller = (uint32_t)__builtin_return_address (0);
  memleaks_log_malloc((uint32_t)ptr,(uint32_t)caller,sz);
#endif

    return ptr;
}

void* calloc (size_t num, size_t size)
{
    size_t total = num * size;
    void * p = usermalloc(total);

#if PROJECT_CONFIG_MEM_LEAKS_CHECKS == 1
    uint32_t caller = (uint32_t)__builtin_return_address (0);
  memleaks_log_malloc((uint32_t)p,(uint32_t)caller,total);
#endif

    if (!p) return NULL;

    return memset(p, 0, total);
}

/* struct tm * localtime(const time_t *t)
{
  static struct tm y = {0};
  //RtcServiceGetCurrentDateTime(&tm);
  return &y;
} */

void *realloc(void *aptr, size_t nbytes)
{
    return userrealloc(aptr, nbytes);
}


void* _calloc_r (struct _reent *r, size_t a, size_t b)
{
    size_t total = a * b;
    void * p = usermalloc(total);

#if PROJECT_CONFIG_MEM_LEAKS_CHECKS == 1
    uint32_t caller = (uint32_t)__builtin_return_address (0);
  memleaks_log_malloc((uint32_t)p,(uint32_t)caller,total);
#endif

    if (!p) return NULL;

    return memset(p, 0, total);
}

void _free_r (struct _reent *r, void* x)
{
#if PROJECT_CONFIG_MEM_LEAKS_CHECKS == 1
    uint32_t caller = (uint32_t)__builtin_return_address (0);
  memleaks_log_free((uint32_t)x,caller);
#endif
    return userfree(x);
}

void* _realloc_r (struct _reent *r, void* x, size_t sz)
{
    return realloc(x, sz);
}

void _putchar(char character)
{
    // Use of printf is banned
}


