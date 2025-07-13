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

APR_API void
apr_g_tuple_add_kvpair (g_tuple_t *tup, g_entry_t *e)
{
  if (tup->kvp_cap == tup->kvp_count)
    {
      tup->kv_pairs = APR_REALLOC (
          tup->kv_pairs, (tup->kvp_count + 1) * sizeof (*tup->kv_pairs));
    }

  tup->kv_pairs[tup->kvp_count++] = e;
}