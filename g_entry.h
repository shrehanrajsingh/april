#if !defined(G_ENTRY_H)
#define G_ENTRY_H

#include "header.h"
#include "mem.h"

enum GEntryType
{
  GENTRY_INTEGER,
  GENTRY_STRING,
  GENTRY_BOOL,
  GENTRY_FLOAT
};

typedef int g_int;
typedef char *g_string;
typedef int g_bool;
typedef float g_float;

struct s_entry
{
  enum GEntryType type;

  union
  {
    g_int f_int;
    g_string f_string;
    g_bool f_bool;
    g_float f_float;
  };
};

typedef struct s_entry g_entry_t;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API g_entry_t *apr_g_entry_new (void);
  APR_API g_entry_t *apr_g_entry_new_with (enum GEntryType _Type, void *_Val);

#if defined(__cplusplus)
}
#endif

#endif // G_ENTRY_H
