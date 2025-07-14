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

// APR_API void
// apr_ctx_table_linktuple (AprContext *ctx, graph_t *tbl, g_tuple_t *tup)
// {
//   graph_t *tuple_mask = NULL;

//   for (size_t i = 0; i < tbl->count_children; i++)
//     {
//       if (tbl->next_children[i]->weights.w_edge == W_TUPLE_MASK)
//         {
//           tuple_mask = tbl->next_children[i];
//           break;
//         }
//     }

//   if (tuple_mask == NULL)
//     {
//       eprintf ("Table has no tuple mask. (It is an abstract table)");
//       return;
//     }

//   graph_t *tup_iter = tuple_mask;

//   graph_t *n_tup = GRAPH (G_TUPLE);
//   n_tup->v.g_tuple = tup;

//   if (!tup_iter->count_children)
//     {
//       ADD_EDGE (tup_iter, n_tup);
//     }
//   else
//     {
//       graph_t *c1 = tup_iter->next_children[0];
//       tup_iter->next_children[0] = n_tup;
//       ADD_EDGE (n_tup, c1);
//     }
// }

APR_API void
apr_ctx_table_linktuple (AprContext *ctx, graph_t *tr, char *entries, ...)
{
  va_list vl;
  va_start (vl, entries);

  g_tuple_t *tup_ref = apr_g_tuple_new ();

  char key_buf[APR_FIELD_NAME_SIZE];
  size_t kc = 0;
  memset (key_buf, 0, sizeof (key_buf));

  int fields_index = apr_graph_get_first_index_by_type (tr, G_FIELD);
  assert (fields_index + 1);

  size_t i = 0;
  while (*entries != '\0')
    {
      char c = *entries++;

      if (c == ',')
        {
          key_buf[kc++] = '\0';
          kc = 0;

          for (int j = fields_index;
               j < tr->count_children && tr->next_children[j]->type == G_FIELD;
               j++)
            {
              if (!strcmp (tr->next_children[fields_index]->v.g_field->name,
                           key_buf))
                {
                  graph_t *type_first = apr_graph_get_first_by_type (
                      tr->next_children[j], G_ENTRY);

                  assert (!!type_first);

                  enum GEntryType ty = type_first->v.g_entry->type;

                  switch (ty)
                    {
                    case GENTRY_BOOL:
                      {
                        g_bool val = (g_bool)va_arg (vl, g_bool);
                        apr_g_tuple_add_kvpair (
                            tup_ref, (struct s_kvp_container){
                                         .key = APR_STRDUP (key_buf),
                                         .val = apr_g_entry_new_with (
                                             ty, apr_g_bool_ptr_new (val)) });
                      }
                      break;
                    case GENTRY_FLOAT:
                      {
                        g_float val = (g_float)va_arg (vl, g_float);
                        apr_g_tuple_add_kvpair (
                            tup_ref, (struct s_kvp_container){
                                         .key = APR_STRDUP (key_buf),
                                         .val = apr_g_entry_new_with (
                                             ty, apr_g_float_ptr_new (val)) });
                      }
                      break;
                    case GENTRY_INTEGER:
                      {
                        g_int val = (g_int)va_arg (vl, g_int);
                        apr_g_tuple_add_kvpair (
                            tup_ref, (struct s_kvp_container){
                                         .key = APR_STRDUP (key_buf),
                                         .val = apr_g_entry_new_with (
                                             ty, apr_g_int_ptr_new (val)) });
                      }
                      break;
                    case GENTRY_STRING:
                      {
                        g_string val = (g_string)va_arg (vl, g_string);
                        apr_g_tuple_add_kvpair (
                            tup_ref, (struct s_kvp_container){
                                         .key = APR_STRDUP (key_buf),
                                         .val = apr_g_entry_new_with (
                                             ty, APR_STRDUP (val)) });
                      }
                      break;

                    default:
                      break;
                    }

                  break;
                }
            }
        }
      else
        {
          key_buf[kc++] = c;
        }
    }

  graph_t *tuple_mask = NULL;

  for (size_t i = 0; i < tr->count_children; i++)
    {
      if (tr->next_children[i]->weights.w_edge == W_TUPLE_MASK)
        {
          tuple_mask = tr->next_children[i];
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
  n_tup->v.g_tuple = tup_ref;

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

  va_end (vl);
}

APR_API void
apr_ctx_destroy (AprContext *ctx)
{
  apr_graph_delete (ctx->g);
  APR_FREE (ctx);
}