#include "g_table.h"

APR_API g_table_t *
apr_g_table_new (void)
{
  g_table_t *t = APR_MALLOC (sizeof (g_table_t));
  memset (t->name, 0, sizeof (t->name));

  return t;
}

APR_API void
apr_g_table_setname (g_table_t *tr, const char *n)
{
  strcpy (tr->name, n);
}