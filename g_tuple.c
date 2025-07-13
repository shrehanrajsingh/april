#include "g_tuple.h"

APR_API g_tuple_t *
apr_g_tuple_new (void)
{
  g_tuple_t *t = APR_MALLOC (sizeof (g_tuple_t));

  t->kvp_cap = G_TUPLE_KVP_SIZE;
  t->kvp_count = 0;
  t->kv_pairs = APR_MALLOC (t->kvp_cap * sizeof (g_entry_t *));

  return t;
}