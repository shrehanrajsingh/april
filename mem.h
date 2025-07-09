#if !defined(MEM_H)
#define MEM_H

#include "header.h"

#include "gc.h"

#if !defined(APR_MALLOC)
#if !defined(APR_USE_GC)
#define APR_MALLOC(X) apr_malloc ((X))
#else
#define APR_MALLOC(X) __gc_malloc ((X))
#endif
#endif // APR_MALLOC

#if !defined(APR_REALLOC)
#if !defined(APR_USE_GC)
#define APR_REALLOC(X, Y) apr_realloc ((X), (Y))
#else
#define APR_REALLOC(X, Y) __gc_realloc ((X), (Y))
#endif
#endif // APR_REALLOC

#if !defined(APR_FREE)
#if !defined(APR_USE_GC)
#define APR_FREE(X) apr_free ((X))
#else
#define APR_FREE(X) __gc_free ((X))
#endif
#endif // APR_FREE

#if !defined(APR_STRDUP)
#if !defined(APR_USE_GC)
#define APR_STRDUP(X) apr_strdup ((X))
#else
#define APR_STRDUP(X) __gc_s_strdup ((X))
#endif
#endif // APR_STRDUP

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API void *apr_malloc (size_t);
  APR_API void *apr_realloc (void *, size_t);
  APR_API void apr_free (void *);

  APR_API char *apr_strdup (const char *);

#if defined(__cplusplus)
}
#endif

#endif // MEM_H
