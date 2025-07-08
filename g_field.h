#if !defined(G_FIELD_H)
#define G_FIELD_H

#include "header.h"
#include "mem.h"

#define APR_FIELD_NAME_SIZE 64

struct s_field
{
  char name[APR_FIELD_NAME_SIZE];
};

typedef struct s_field g_field_t;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API g_field_t *apr_g_field_new (void);
  APR_API g_field_t *apr_g_field_new_with (char *_Name);

#if defined(__cplusplus)
}
#endif

#endif // G_FIELD_H
