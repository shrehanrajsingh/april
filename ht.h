#if !defined(HASH_TABLE_H)
#define HASH_TABLE_H

#include "header.h"
#include "mem.h"

/**
 * This is only a reference (a minimum)
 * The actual size is nextPrime(5000) of the form 4k + 3
 */
#define HASH_TABLE_SIZE 5000

/**
 * Basic hash table
 * Quadratic Probing
 * Might change to Hopscotch Hashing in the future
 *
 * Table size: prime of the form 4k + 3
 * Collision resolution strategy: i**2, -i**2
 */

struct s_hasht
{
  void **entries; /* malloc of void * */
  size_t size;

  int (*f1) (int); /* collision resolution strategy 1 (i**2) */
  int (*f2) (int); /* collision resolution strategy 2 (-i**2) */
};

typedef struct s_hasht hash_t;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API hash_t *apr_hash_table_new ();

#if defined(__cplusplus)
}
#endif

#endif // HASH_TABLE_H
