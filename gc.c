#include "gc.h"

#if defined(APR_USE_GC)

static GCObject *gc_objects = NULL;
size_t total_allocated = 0;
size_t threshold = 1024 * 1024; /* 1 MB */

void gc_mark_all (void *);
void gc_sweep ();

APR_API void
apr_gc_init (void)
{
  /*  */
}

APR_API void *
__gc_malloc (size_t s)
{
  void *ptr = malloc (s);
  if (!ptr)
    {
      eprintf ("__gc_malloc(): memory allocation failed");
      return NULL;
    }

  GCObject *obj = malloc (sizeof (GCObject));
  obj->ptr = ptr;
  obj->size = s;
  obj->marked = 0;
  obj->next = gc_objects;
  gc_objects = obj;

  total_allocated += obj->size;

  if (total_allocated > threshold)
    {
      apr_gc_collect ();
    }

  return ptr;
}

APR_API void *
__gc_realloc (void *old, size_t ns)
{
  void *n = realloc (old, ns);
  if (!n)
    {
      eprintf ("__gc_realloc(): memory allocation failed");
      return NULL;
    }

  GCObject *goc = gc_objects;
  while (goc != NULL && goc->ptr != old)
    goc = goc->next;

  if (goc == NULL)
    {
      eprintf ("__gc_realloc(): pointer to reallocate was not allocated");
      return NULL;
    }

  goc->size = ns;
  goc->ptr = n;
  goc->marked = 0;
  total_allocated += goc->size;

  if (total_allocated > threshold)
    {
      apr_gc_collect ();
    }

  return n;
}

APR_API void
__gc_free (void *ptr)
{
  GCObject *goc = gc_objects, *last_obj = NULL;

  while (goc != NULL && goc->ptr != ptr)
    {
      last_obj = goc;
      goc = goc->next;
    }

  if (goc == NULL)
    {
      eprintf ("__gc_free(): pointer to free was not allocated");
      return;
    }

  total_allocated -= goc->size;
  if (last_obj != NULL)
    last_obj->next = goc->next;
  else
    gc_objects = goc->next;

  free (ptr);
  free (goc);
}

APR_API char *
__gc_strdup (const char *s)
{
  char *r = __gc_malloc (strlen (s) + 1);
  strcpy (r, s);
  return r;
}

APR_API void
apr_gc_collect (void)
{
  uintptr_t stack_v;
  gc_mark_all (&stack_v);
  gc_sweep ();
}

void
gc_mark_all (void *p)
{
  uintptr_t *sp = (uintptr_t *)p;
  uintptr_t *end = (uintptr_t *)__builtin_frame_address (0);

  for (; sp > end; sp--)
    {
      uintptr_t val = *sp;

      for (GCObject *obj = gc_objects; obj; obj = obj->next)
        {
          if ((uintptr_t)obj->ptr <= val
              && val < (uintptr_t)obj->ptr + obj->size)
            {
              obj->marked = 1;
            }
        }
    }
}

void
gc_sweep ()
{
  GCObject **objs = &gc_objects;

  while (*objs)
    {
      if ((*objs)->marked)
        {
          total_allocated -= (*objs)->size;
          free ((*objs)->ptr);
          GCObject *unreached = *objs;
          *objs = unreached->next;
          free (unreached);
        }
      else
        {
          (*objs)->marked = 0;
          objs = &(*objs)->next;
        }
    }
}

#endif
