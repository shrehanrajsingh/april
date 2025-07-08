#include "g_field.h"

APR_API g_field_t *
apr_g_field_new (void)
{
  g_field_t *f = APR_MALLOC (sizeof (g_field_t));
  memset (f->name, 0, sizeof (f->name));

  return f;
}

APR_API g_field_t *
apr_g_field_new_with (char *n)
{
  g_field_t *f = apr_g_field_new ();
  strcpy (f->name, n);

  return f;
}