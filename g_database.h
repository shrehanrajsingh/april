#if !defined(G_DATABASE_H)
#define G_DATABASE_H

#include "g_table.h"
#include "header.h"

#define APR_DATABASE_NAME_SIZE 64

struct s_database
{
  char name[APR_DATABASE_NAME_SIZE];

  g_table_t *tbls;
  size_t
      tb_count; /* tables are added rarely so we can omit count/cap paradigm */
};

typedef struct s_database g_db_t;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

#if defined(__cplusplus)
}
#endif

#endif // G_DATABASE_H
