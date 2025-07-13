#if !defined(GTUPLE_H)
#define GTUPLE_H

#include "g_entry.h"
#include "header.h"

#define G_TUPLE_KVP_SIZE (32)

struct s_tuple
{
  /**
   * a reference to the actual field
   * will be stored in next_children,
   * this kv_pairs is an array which
   * will derive from those references
   * with the difference being that
   * they will also store a value
   * (fields store defaults of a value)
   */
  g_entry_t **kv_pairs;
  size_t kvp_cap;
  size_t kvp_count;
};

typedef struct s_tuple g_tuple_t;

#endif // GTUPLE_H
