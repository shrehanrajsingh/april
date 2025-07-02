#include "mem.h"

APR_API void *
apr_malloc (size_t s)
{
  if (!s)
    return NULL;

  void *p = malloc (s);
  assert (p && "apr_malloc() failed");

  return p;
}

APR_API void *
apr_realloc (void *old, size_t n)
{
  if (n == 0)
    {
      apr_free (old);
      return NULL;
    }

  old = realloc (old, n);
  assert (old && "apr_realloc() failed");

  return old;
}

APR_API void
apr_free (void *p)
{
  if (p != NULL)
    free (p);
}

APR_API char *
apr_strdup (const char *s)
{
  size_t sl = strlen (s);

  char *r = APR_MALLOC (sl);
  memcpy (r, s, sl);

  return r;
}