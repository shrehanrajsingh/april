#include "graph.h"

static void resize_children (graph_t *);
void free_v (graph_t *); /* call custom free routine on graph_t.v */

APR_API graph_t *
apr_graph_new (enum GraphType gt, int is_root)
{
  graph_t *t = APR_MALLOC (sizeof (graph_t));

  t->type = gt;
  t->cap_children = GRAPH_CHILDREN_CAPACITY;
  t->count_children = 0;
  t->is_root = is_root;
  t->next_children = APR_MALLOC (t->cap_children * sizeof (graph_t *));

  return t;
}

APR_API void
apr_graph_add_edge (graph_t *p, graph_t *e)
{
  if (p->cap_children == p->count_children)
    resize_children (p);

  size_t e_idx = 0;

  while (e_idx < p->count_children
         && p->next_children[e_idx]->weights.w_edge
                <= e->weights.w_edge) /* <= so new edges are favoured more */
    e_idx++;

  if (e_idx < p->count_children)
    {
      memmove (&p->next_children[e_idx + 1], &p->next_children[e_idx],
               (p->count_children - e_idx) * sizeof (graph_t *));
    }

  p->next_children[e_idx] = e;
  p->count_children++;
}

void
resize_children (graph_t *g)
{
  g->cap_children += GRAPH_CHILDREN_CAPACITY;
  g->next_children
      = APR_REALLOC (g->next_children, g->cap_children * sizeof (graph_t *));
}

APR_API void
apr_graph_delete (graph_t *g)
{
  if (g->count_children)
    {
      while (g->count_children--)
        apr_graph_delete (*g->next_children++);
    }

  free_v (g);

  APR_FREE (g->next_children);
  APR_FREE (g);
}

void
free_v (graph_t *g)
{
  switch (g->type)
    {
    case G_DATABASE:
      {
        /* TODO */
      }
      break;
    case G_TABLE:
      {
        /* TODO */
      }
      break;
    case G_FIELD:
      {
        /* TODO */
      }
      break;
    case G_CONSTRAINT:
      {
        /* TODO */
      }
      break;
    case G_TYPE:
      {
        /* TODO */
      }
      break;
    case G_TUPLE:
      {
        /* TODO */
      }
      break;
    case G_ENTRY:
      {
        /* TODO */
      }
      break;

    default:
      break;
    }
}