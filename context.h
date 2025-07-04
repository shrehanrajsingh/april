#if !defined(CONTEXT_H)
#define CONTEXT_H

#include "header.h"

struct s_apr_context
{
  int i;
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
