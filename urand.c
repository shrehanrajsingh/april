#include "urand.h"

APR_API uniform_random_t
apr_uniform_random_new (void)
{
  uniform_random_t t;
  t.seed = time (NULL);

  return t;
}

APR_API uniform_random_t
apr_uniform_random_new_with (long long s)
{
  uniform_random_t t;
  t.seed = s;

  return t;
}

APR_API int
apr_uniform_random_next_int (uniform_random_t *u)
{
  srand (u->seed);
  return rand ();
}