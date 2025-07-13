#include "g_entry.h"

APR_API g_entry_t *
apr_g_entry_new (void)
{
  g_entry_t *f = APR_MALLOC (sizeof (g_entry_t));
  return f;
}

APR_API g_entry_t *
apr_g_entry_new_with (enum GEntryType t, void *v)
{
  g_entry_t *f = apr_g_entry_new ();
  f->type = t;

  switch (f->type)
    {
    case GENTRY_INTEGER:
      f->f_int = v != NULL ? *(g_int *)v : 0;
      break;
    case GENTRY_BOOL:
      f->f_int = v != NULL ? *(g_bool *)v : 0;
      break;
    case GENTRY_FLOAT:
      f->f_int = v != NULL ? *(g_float *)v : 0.0f;
      break;
    case GENTRY_STRING:
      f->f_string = v != NULL ? (g_string)v : "";
      break;

    default:
      eprintf ("invalid type '%d'", f->type);
      break;
    }

  return f;
}

APR_API g_int *
apr_g_int_ptr_new (g_int v)
{
  g_int *r = APR_MALLOC (sizeof (*r));
  *r = v;

  return r;
}

APR_API g_bool *
apr_g_bool_ptr_new (g_bool v)
{
  g_bool *r = APR_MALLOC (sizeof (*r));
  *r = v;

  return r;
}

APR_API g_float *
apr_g_float_ptr_new (g_float v)
{
  g_float *r = APR_MALLOC (sizeof (*r));
  *r = v;

  return r;
}