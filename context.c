#include "context.h"

APR_API AprContext *
apr_ctx_new (void)
{
  AprContext *ac = APR_MALLOC (sizeof (AprContext));
  ac->g = GRAPH (G_DATABASE);
  ac->g->is_root = 1;

  ac->g->v.g_database = apr_g_database_new ();

  return ac;
}

APR_API void
apr_ctx_set_dbname (AprContext *ctx, const char *n)
{
  strcpy (ctx->g->v.g_database->name, n);
}

APR_API graph_t *
apr_ctx_db_linktable (AprContext *ctx, const char *n)
{
  graph_t *table_gr = GRAPH (G_TABLE);

  table_gr->v.g_table = apr_g_table_new ();
  apr_g_table_setname (table_gr->v.g_table, n);

  table_gr->weights.w_edge = W_TABLE;

  ADD_EDGE (ctx->g, table_gr);
  return table_gr;
}

APR_API void
apr_ctx_table_linkfield (AprContext *ctx, graph_t *tr, const char *fname,
                         enum GEntryType type)
{
  graph_t *field_gr = GRAPH (G_FIELD);
  graph_t *entry_gr = GRAPH (G_ENTRY);

  field_gr->v.g_field = apr_g_field_new_with ((char *)fname);
  entry_gr->v.g_entry = apr_g_entry_new_with (type, NULL);

  field_gr->weights.w_edge = W_FIELD;
  entry_gr->weights.w_edge = W_ENTRY;

  ADD_EDGE (field_gr, entry_gr); /* connect field name with its type */
  ADD_EDGE (tr, field_gr);       /* connect field with its table */
}

APR_API void
apr_ctx_table_linktuplemask (AprContext *ctx, graph_t *tr)
{
  for (size_t i = 0; i < tr->count_children; i++)
    {
      if (tr->next_children[i]->weights.w_edge == W_TUPLE_MASK)
        {
          eprintf ("table already has a tuple mask");
          return;
        }
    }

  graph_t *tme = GRAPH (G_TUPLE_MASK);
  tme->weights.w_edge = W_TUPLE_MASK;

  ADD_EDGE (tr, tme);
}

APR_API void
apr_ctx_table_linktuple (AprContext *ctx, graph_t *tbl, g_tuple_t *tup)
{
  graph_t *tuple_mask = NULL;

  for (size_t i = 0; i < tbl->count_children; i++)
    {
      if (tbl->next_children[i]->weights.w_edge == W_TUPLE_MASK)
        {
          tuple_mask = tbl->next_children[i];
          break;
        }
    }

  if (tuple_mask == NULL)
    {
      eprintf ("Table has no tuple mask. (It is an abstract table)");
      return;
    }

  graph_t *tup_iter = tuple_mask;

  graph_t *n_tup = GRAPH (G_TUPLE);
  n_tup->v.g_tuple = tup;

  if (!tup_iter->count_children)
    {
      ADD_EDGE (tup_iter, n_tup);
    }
  else
    {
      graph_t *c1 = tup_iter->next_children[0];
      tup_iter->next_children[0] = n_tup;
      ADD_EDGE (n_tup, c1);
    }
}

APR_API void
apr_ctx_destroy (AprContext *ctx)
{
  apr_graph_delete (ctx->g);
  APR_FREE (ctx);
}