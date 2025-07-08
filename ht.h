#if !defined(HASH_TABLE_H)
#define HASH_TABLE_H

#include "g_entry.h"
#include "header.h"
#include "mem.h"
#include "urand.h"

/**
 * This is only a reference (a minimum)
 * The actual size is nextPrime(5000) of the form 4k + 3
 */
#define HASH_TABLE_SIZE (5000)

#define HASH(X, Y) _apr_hash ((X), (Y))
#define MP_31 (0x7FFFFFFF)

/**
 * Basic hash table
 * Quadratic Probing
 * Might change to Hopscotch Hashing in the future
 *
 * Table size: prime of the form 4k + 3
 * Collision resolution strategy: i**2, -i**2
 */

enum HashStatusEnum
{
  INACTIVE,
  ACTIVE,
  DELETED
};

struct s_hasht
{
  void **entries; /* malloc of void * */
  size_t size;

  enum HashStatusEnum *status;
  uniform_random_t u;

  /* H(A, B, x) = ((Ax + B) mod p) mod M */
  int A;
  int B;
  int p;

  int (*f1) (int); /* collision resolution strategy 1 (i**2) */
  int (*f2) (int); /* collision resolution strategy 2 (-i**2) */
};

typedef struct s_hasht hash_t;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  APR_API hash_t *apr_hash_table_new ();
  APR_API void apr_hash_table_destroy (hash_t *);

  APR_API unsigned long apr_hash_str (hash_t *, g_string);
  APR_API unsigned long apr_hash_int (hash_t *, g_int);
  APR_API unsigned long apr_hash_bool (hash_t *, g_bool);
  APR_API unsigned long apr_hash_float (hash_t *, g_float);
  APR_API unsigned long _apr_hash (hash_t *, g_entry_t *);

#if defined(__cplusplus)
}
#endif

#endif // HASH_TABLE_H
