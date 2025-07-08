#include "ht.h"

static size_t next_prime_form_4k_plus_3 (size_t);

int crs1 (int);
int crs2 (int);

APR_API hash_t *
apr_hash_table_new ()
{
  hash_t *h = APR_MALLOC (sizeof (hash_t));
  h->size = next_prime_form_4k_plus_3 (HASH_TABLE_SIZE);

  h->entries = APR_MALLOC (h->size * sizeof (*h->entries));
  h->status = APR_MALLOC (h->size * sizeof (*h->status));

  memset (h->entries, 0, h->size * sizeof (void *));
  memset (h->status, INACTIVE, h->size * sizeof (enum HashStatusEnum));

  h->f1 = &crs1;
  h->f2 = &crs2;

  return h;
}

static size_t
next_prime_form_4k_plus_3 (size_t m)
{
  size_t k = (m - 3) >> 2;
  size_t p = (k << 2) + 3;

l1:
  int is_prime = 1;
  for (int i = 2; is_prime && i < p / 2; ++i)
    {
      if (!(p % i))
        is_prime = 0;
    }

  if (!is_prime)
    {
      k++;
      p = (k << 2) + 3;
      goto l1;
    }

  return p;
}

int
crs1 (int i)
{
  return i * i;
}

int
crs2 (int i)
{
  return -(i * i);
}

APR_API void
apr_hash_destroy (hash_t *h)
{
  APR_FREE (h->entries);
  APR_FREE (h->status);
  APR_FREE (h);
}