#if !defined(MEM_H)
#define MEM_H

#include "header.h"

#if !defined(APR_MALLOC)
#define APR_MALLOC(X) apr_malloc ((X))
#endif // APR_MALLOC

#if !defined(APR_REALLOC)
#define APR_REALLOC(X, Y) apr_realloc ((X), (Y))
#endif // APR_REALLOC

#if !defined(APR_FREE)
#define APR_FREE(X) apr_free ((X))
#endif // APR_FREE

#if !defined(APR_STRDUP)
#define APR_STRDUP(X) apr_strdup ((X))
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
