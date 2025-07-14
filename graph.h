#if !defined(GRAPH_H)
#define GRAPH_H

#include "header.h"
#include "mem.h"

#include "g_database.h"
#include "g_entry.h"
#include "g_field.h"
#include "g_table.h"
#include "g_tuple.h"

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

#if !defined(EDGE_TYPE)
#define EDGE_TYPE(X)                                                          \
  ((X) == G_DATABASE       ? "G_DATABASE"                                     \
   : (X) == G_TABLE        ? "G_TABLE"                                        \
   : (X) == G_FIELD        ? "G_FIELD"                                        \
   : (X) == G_CONSTRAINT   ? "G_CONSTRAINT"                                   \
   : (X) == G_TYPE         ? "G_TYPE"                                         \
   : (X) == G_TUPLE_MASK   ? "G_TUPLE_MASK"                                   \
   : (X) == G_TUPLE        ? "G_TUPLE"                                        \
   : (X) == G_TYPEREF_MASK ? "G_TYPEREF_MASK"                                 \
   : (X) == G_ENTRY        ? "G_ENTRY"                                        \
                           : "UNKNOWN")
#endif // EDGE_TYPE

#define GRAPH_CHILDREN_CAPACITY (8)

#define W_TABLE 0
#define W_TUPLE_MASK 0
#define W_TUPLE                                                               \
  0 /* 0 because it's wrt tuple_mask and tuple, which only stores tuple       \
     */
#define W_FIELD 1
#define W_ENTRY 0

struct s_graph
{
  enum GraphType type;

  union
  {
    /* no field for masks */
    g_db_t *g_database;
    g_table_t *g_table;
    g_field_t *g_field;
    void *g_constraint;
    g_tuple_t *g_tuple;
    g_entry_t *g_entry;
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
#define GRAPH(X) (apr_graph_new ((X), 0))
#endif // GRAPH

#if !defined(ADD_EDGE)
#define ADD_EDGE(X, Y) (apr_graph_add_edge ((X), (Y)))
#endif // ADD_EDGE

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API graph_t *apr_graph_new (enum GraphType _Type, int _IsRoot);

  APR_API void apr_graph_add_edge (graph_t *_Parent, graph_t *_Edge);

  APR_API void apr_graph_delete (graph_t *_G);

  /* Defined as the sum of the heights of all children, the number of children,
   * and one for the root node. */
  APR_API size_t apr_graph_height (graph_t *_G);

  APR_API graph_t *apr_graph_get_first_by_type (graph_t *_G,
                                                enum GraphType _Type);

  APR_API int apr_graph_get_first_index_by_type (graph_t *_G,
                                                 enum GraphType _Type);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // GRAPH_H
