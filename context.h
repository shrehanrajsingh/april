#if !defined(CONTEXT_H)
#define CONTEXT_H

#include "g_database.h"
#include "g_table.h"
#include "header.h"

struct s_apr_context
{
  g_db_t *dbs;
  size_t db_count; /* databases are added seldom so we can compromise on
                      reallocs every time a database is added */
};

typedef struct s_apr_context AprContext;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

#if defined(__cplusplus)
}
#endif

#endif // CONTEXT_H
