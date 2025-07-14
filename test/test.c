#include <april.h>

#define TEST(X) test##X ()

void
test1 ()
{
  graph_t *g = GRAPH (G_ENTRY);
  g->is_root = 1;

  apr_graph_add_edge (g, GRAPH (G_ENTRY));
  apr_graph_add_edge (g, apr_graph_new (G_CONSTRAINT, 0));

  graph_t **st = APR_MALLOC (apr_graph_height (g) * sizeof (graph_t *));
  *st = g;
  size_t s = 1, c = 0;

  printf ("graph height: %ld\n", apr_graph_height (g));
  while (s > 0)
    {
      graph_t *p = st[--s];

      printf ("edge %ld: %d {children: %ld, is_root: %d}\n", c++, p->type,
              p->count_children, p->is_root);
      for (int i = 0; i < p->count_children; i++)
        st[s++] = p->next_children[i];
    }

  APR_FREE (st);
}

void
test2 ()
{
  graph_t *g_db = GRAPH (G_DATABASE), *g_table, *g_tuple, *g_fid, *g_funame,
          *g_type_fid, *g_type_funame;

  g_db->is_root = 1;

  g_table = GRAPH (G_TABLE);
  g_table->weights.w_edge = W_TABLE; /* with respect to database */

  ADD_EDGE (g_db, g_table);

  g_tuple = GRAPH (G_TUPLE_MASK);
  g_tuple->weights.w_edge = W_TUPLE_MASK; /* with respect to table */

  ADD_EDGE (g_table, g_tuple);

  g_fid = GRAPH (G_FIELD);
  g_fid->weights.w_edge = W_FIELD; /* with respect to table */

  g_type_fid = GRAPH (G_ENTRY);
  g_type_fid->weights.w_edge = W_ENTRY; /* with respect to g_fid */

  ADD_EDGE (g_fid, g_type_fid);
  ADD_EDGE (g_table, g_fid);

  g_funame = GRAPH (G_FIELD);
  g_funame->weights.w_edge = W_FIELD; /* with respect to table */

  g_type_funame = GRAPH (G_ENTRY);
  g_type_funame->weights.w_edge = W_ENTRY; /* with respect to g_funame */

  ADD_EDGE (g_funame, g_type_funame);
  ADD_EDGE (g_table, g_funame);

  graph_t **st = APR_MALLOC (apr_graph_height (g_db) * sizeof (graph_t *));
  *st = g_db;
  size_t s = 1, c = 0;

  printf ("graph height: %ld\n", apr_graph_height (g_db));
  while (s > 0)
    {
      graph_t *p = st[--s];

      printf ("edge %ld: %s {children: %ld, is_root: %d}\n", c++,
              EDGE_TYPE (p->type), p->count_children, p->is_root);
      for (int i = 0; i < p->count_children; i++)
        st[s++] = p->next_children[i];
    }

  APR_FREE (st);
}

void
test3 ()
{
  hash_t *h = apr_hash_table_new ();

  unsigned long hashlist[] = {
    /* int hashtest */
    apr_hash_int (h, 1),  apr_hash_int (h, 24),    apr_hash_int (h, 7),
    apr_hash_int (h, 19), apr_hash_int (h, 12293), apr_hash_int (h, 1511),
  };

  for (int i = 0; i < sizeof (hashlist) / sizeof (hashlist[0]); i++)
    printf ("%ld\n", hashlist[i]);

  /* brute force collision check */
  unsigned long *memtable = APR_MALLOC (h->size * sizeof (*memtable));
  size_t *idcs = APR_MALLOC (h->size * sizeof (*idcs));
  size_t mc = 0;
  size_t num_coll = 0;

  for (int i = 1; i < h->size; i++)
    {
      unsigned long k = apr_hash_int (h, i);
      for (int j = 0; j < mc; j++)
        {
          if (memtable[j] == k)
            {
              // printf ("collision found: '%d' with '%d' (hash: %ld)\n", j, i,
              //         k);
              num_coll++;
              break;
            }
        }

      memtable[mc] = k;
      memtable[mc++] = i;
    }

  printf ("total integer collisions: %ld against table size of %ld\n",
          num_coll, h->size);

  APR_FREE (idcs);
  APR_FREE (memtable);

  /* string hashes */
  const g_string str_names[]
      = { "id", "username", "password", "email", "created_at", NULL };

  for (int i = 0; str_names[i] != NULL; i++)
    {
      printf ("string '%s' has hash %ld\n", str_names[i],
              apr_hash_str (h, str_names[i]));
    }

  /* adding keys */
  g_int id_v = 10;
  g_entry_t *e_id;

  g_string uname = "shrehan";
  g_entry_t *e_uname;

  g_string password = "nicetry";
  g_entry_t *e_password;

  g_string email = "goodemail@provider.com";
  g_entry_t *e_email;

  apr_hash_add_key (h, e_id = apr_g_entry_new_with (GENTRY_INTEGER, "id"),
                    (void *)&id_v);

  apr_hash_add_key (h, e_uname = apr_g_entry_new_with (GENTRY_STRING, "uname"),
                    (void *)&uname);

  apr_hash_add_key (
      h, e_password = apr_g_entry_new_with (GENTRY_STRING, "password"),
      (void *)&password);

  apr_hash_add_key (h, e_email = apr_g_entry_new_with (GENTRY_STRING, "email"),
                    (void *)&email);

  printf ("key 'id' has value %d\n", *(g_int *)apr_hash_get (h, e_id, NULL));

  printf ("key 'uname' has value '%s'\n",
          *(g_string *)apr_hash_get (h, e_uname, NULL));

  printf ("key 'password' has value '%s'\n",
          *(g_string *)apr_hash_get (h, e_password, NULL));

  printf ("key 'email' has value '%s'\n",
          *(g_string *)apr_hash_get (h, e_email, NULL));

  APR_FREE (e_id);
  APR_FREE (e_uname);
  APR_FREE (e_password);
  APR_FREE (e_email);

  apr_hash_table_destroy (h);
  h = NULL;
}

/* test4 to 8 are reserved for future features */

void
test9 ()
{
  AprContext *ctx = apr_ctx_new ();
  apr_ctx_set_dbname (ctx, "main");

  /* API guidelines
    apr_ctx_db_linktable (ctx, table_name) // returns a table ref
    apr_ctx_table_linkfield (ctx, table_ref, field_name, field_type)
  */
}

void
test10 ()
{
  AprContext *ctx = apr_ctx_new ();
  apr_ctx_set_dbname (ctx, "hellodb");

  graph_t *tr = apr_ctx_db_linktable (ctx, "hellotable");
  apr_ctx_table_linkfield (ctx, tr, "id", GENTRY_INTEGER);
  apr_ctx_table_linkfield (ctx, tr, "username", GENTRY_STRING);
  apr_ctx_table_linkfield (ctx, tr, "password", GENTRY_STRING);
  apr_ctx_table_linkfield (ctx, tr, "email", GENTRY_STRING);
  apr_ctx_table_linktuplemask (ctx, tr);

  // g_tuple_t *tup_ref = apr_g_tuple_new ();
  // apr_g_tuple_add_kvpair (
  //     tup_ref, apr_g_entry_new_with (GENTRY_INTEGER, apr_g_int_ptr_new
  //     (1)));
  // apr_g_tuple_add_kvpair (
  //     tup_ref, apr_g_entry_new_with (
  //                  GENTRY_STRING, (g_string)APR_STRDUP
  //                  ("shrehanrajsingh")));
  // apr_g_tuple_add_kvpair (
  //     tup_ref, apr_g_entry_new_with (
  //                  GENTRY_STRING, (g_string)APR_STRDUP
  //                  ("somecoolpassword")));
  // apr_g_tuple_add_kvpair (
  //     tup_ref, apr_g_entry_new_with (GENTRY_STRING,
  //                                    (g_string)
  //                                    "somecoolemail@provider.com"));

  // apr_ctx_table_linktuple (ctx, tr, tup_ref);

  apr_ctx_table_linktuple (ctx, tr, "id,username,password,email", 1,
                           "shrehanrajsingh", "sonecoolpassword",
                           "somecoolemail@provider.com");

  graph_t **st = APR_MALLOC (apr_graph_height (ctx->g) * sizeof (graph_t *));
  *st = ctx->g;
  size_t s = 1, c = 0;

  printf ("graph height: %ld\n", apr_graph_height (ctx->g));
  while (s > 0)
    {
      graph_t *p = st[--s];

      printf ("edge %ld: %s {children: %ld, is_root: %d, edge_weight: %d}\n",
              c++, EDGE_TYPE (p->type), p->count_children, p->is_root,
              p->weights.w_edge);
      for (int i = 0; i < p->count_children; i++)
        st[s++] = p->next_children[i];
    }

  APR_FREE (st);

  apr_ctx_destroy (ctx);
}

int
main (int argc, char const *argv[])
{
  printf ("Hello, from April!\n");

  // printf ("TEST::1\n");
  // TEST (1);
  // printf ("TEST::2\n");
  // TEST (2);
  // printf ("TEST::3\n");
  // TEST (3);
  printf ("TEST::10\n");
  TEST (10);

  return 0;
}