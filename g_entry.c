#include "g_entry.h"

APR_API g_entry_t *
apr_g_entry_new (void)
{
  g_entry_t *f = APR_MALLOC (sizeof (g_entry_t));
  return f;
}

APR_API g_entry_t *
apr_g_entry_new_with (char *n, enum GEntryType t, void *v)
{
  g_entry_t *f = apr_g_entry_new ();
  f->type = t;

  switch (f->type)
    {
    case GENTRY_INTEGER:
      f->f_int = *(g_int *)v;
      break;
    case GENTRY_BOOL:
      f->f_int = *(g_bool *)v;
      break;
    case GENTRY_FLOAT:
      f->f_int = *(g_float *)v;
      break;
    case GENTRY_STRING:
      f->f_string = *(g_string *)v;
      break;

    default:
      eprintf ("invalid type '%d'", f->type);
      break;
    }

  return f;
}