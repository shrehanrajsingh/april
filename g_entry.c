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