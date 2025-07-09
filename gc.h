#if !defined(GC_H)
#define GC_H

#include "header.h"

#define APR_USE_GC

/* use only if gc support is enabled */
#if defined(APR_USE_GC)

struct s_gc_chunk_data
{
  void *ptr;
  size_t size;
  int marked;
  struct s_gc_chunk_data *next;
};

typedef struct s_gc_chunk_data GCObject;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API void apr_gc_init (void);

  APR_API void *__gc_malloc (size_t _Size);
  APR_API void *__gc_realloc (void *_Old, size_t _NewSize);
  APR_API void __gc_free (void *_Ptr);
  APR_API char *__gc_strdup (const char *__Str);

  APR_API void apr_gc_collect (void);

#if defined(__cplusplus)
}
#endif

#endif

#endif // GC_H
