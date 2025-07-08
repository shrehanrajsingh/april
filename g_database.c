#include "g_database.h"

APR_API g_db_t *
apr_g_database_new (void)
{
  g_db_t *d = APR_MALLOC (sizeof (g_db_t));

  memset (d->name, 0, sizeof (d->name));
  d->tb_count = 0;
  d->tbls = NULL;

  return d;
}