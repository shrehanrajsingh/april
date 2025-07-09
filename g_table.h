#if !defined(G_TABLE_H)
#define G_TABLE_H

#include "header.h"
#include "mem.h"

#define APR_TABLE_NAME_SIZE 64

struct s_table
{
  char name[APR_TABLE_NAME_SIZE];
};

typedef struct s_table g_table_t;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API g_table_t *apr_g_table_new (void);
  APR_API void apr_g_table_setname (g_table_t *_TableRef, const char *_Name);

#if defined(__cplusplus)
}
#endif

#endif // G_TABLE_H
