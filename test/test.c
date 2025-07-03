#include <april.h>

#define TEST(X) test##X ()

void
test1 ()
{
  graph_t *g = GRAPH ();
  g->is_root = 1;

  apr_graph_add_edge (g, GRAPH ());
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

int
main (int argc, char const *argv[])
{
  printf ("Hello, from April!\n");
  TEST (1);
  return 0;
}