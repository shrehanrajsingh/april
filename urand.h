#if !defined(URAND_H)
#define URAND_H

#include "header.h"
#include "mem.h"

struct s_urand
{
  long long seed;
};

typedef struct s_urand uniform_random_t;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API uniform_random_t apr_uniform_random_new (void);
  APR_API uniform_random_t apr_uniform_random_new_with (long long _Seed);
  APR_API int apr_uniform_random_next_int (uniform_random_t *);

#if defined(__cplusplus)
}
#endif

#endif // URAND_H
