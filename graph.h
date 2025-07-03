#if !defined(GRAPH_H)
#define GRAPH_H

#include "header.h"
#include "mem.h"

enum GraphType
{
  G_DATABASE = 0,
  G_TABLE = 1,
  G_FIELD = 2,
  G_CONSTRAINT = 3,
  G_TYPE = 4,
  G_TUPLE_MASK = 5, /* <table edge> ---> <tuple_mask> ---> <tuple 1> ... */
  G_TUPLE = 6,
  G_TYPEREF_MASK = 7, /* <tuple> ---> <type_ref_mask> ---> <entry> */
  G_ENTRY = 8
};

static const int GRAPH_CHILDREN_CAPACITY = 8;

struct s_graph
{
  enum GraphType type;

  union
  {
    /* no field for masks */
    void *g_database;
    void *g_table;
    void *g_field;
    void *g_constraint;
    void *g_type;
    void *g_tuple;
    void *g_entry;
  } v;

  int is_root;

  struct
  {
    /**
     * w_edge denotes the order in which children will be arranged (sorted)
     * when inserted. For example:
     *   - A G_DATABASE will favor G_TABLE edges above others, so it will have
     * a lower weight. (Lower weight means a lower index.)
     *   - A G_TABLE will favor G_TUPLE_MASK before fields.
     *     (Ordered structures are always favored above unordered ones.)
     */
    int w_edge;
    int w_hash;

  } weights;

  struct s_graph **next_children;
  size_t count_children;
  size_t cap_children;
};

typedef struct s_graph graph_t;

#if !defined(GRAPH)
#define GRAPH() (apr_graph_new (G_ENTRY, 0))
#endif // GRAPH

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API graph_t *apr_graph_new (enum GraphType _Type, int _IsRoot);

  APR_API void apr_graph_add_edge (graph_t *_Parent, graph_t *_Edge);

  APR_API void apr_graph_delete (graph_t *_G);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // GRAPH_H
