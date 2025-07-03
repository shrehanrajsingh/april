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
  g_table->weights.w_edge = 0; /* with respect to database */

  ADD_EDGE (g_db, g_table);

  g_tuple = GRAPH (G_TUPLE_MASK);
  g_tuple->weights.w_edge = 0; /* with respect to table */

  ADD_EDGE (g_table, g_tuple);

  g_fid = GRAPH (G_FIELD);
  g_fid->weights.w_edge = 1; /* with respect to table */

  g_type_fid = GRAPH (G_ENTRY);
  g_type_fid->weights.w_edge = 0; /* with respect to g_fid */

  ADD_EDGE (g_fid, g_type_fid);
  ADD_EDGE (g_table, g_fid);

  g_funame = GRAPH (G_FIELD);
  g_funame->weights.w_edge = 2; /* with respect to table */

  g_type_funame = GRAPH (G_ENTRY);
  g_type_funame->weights.w_edge = 0; /* with respect to g_funame */

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

int
main (int argc, char const *argv[])
{
  printf ("Hello, from April!\n");

  printf ("TEST::1\n");
  TEST (1);
  printf ("TEST::2\n");
  TEST (2);
  return 0;
}