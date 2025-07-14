#if !defined(CONTEXT_H)
#define CONTEXT_H

#include "g_database.h"
#include "g_table.h"
#include "g_tuple.h"
#include "graph.h"
#include "header.h"
#include "mem.h"

struct s_apr_context
{
  graph_t *g;
};

typedef struct s_apr_context AprContext;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API AprContext *apr_ctx_new (void);

  APR_API void apr_ctx_set_dbname (AprContext *_Ctx, const char *_Name);
  APR_API graph_t *apr_ctx_db_linktable (AprContext *_Ctx, const char *_Name);
  APR_API void apr_ctx_table_linkfield (AprContext *_Ctx, graph_t *_TableRef,
                                        const char *_FieldName,
                                        enum GEntryType);

  APR_API void apr_ctx_table_linktuplemask (AprContext *_Ctx,
                                            graph_t *_TableRef);

  APR_API void apr_ctx_table_linktuple (AprContext *_Ctx, graph_t *_TableRef,
                                        char *_Entries, ...);

  APR_API void apr_ctx_destroy (AprContext *);

#if defined(__cplusplus)
}
#endif

#endif // CONTEXT_H
