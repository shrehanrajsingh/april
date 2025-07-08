#include "ht.h"

static size_t next_prime_form_4k_plus_3 (size_t);

int crs1 (int);
int crs2 (int);

APR_API hash_t *
apr_hash_table_new ()
{
  hash_t *h = APR_MALLOC (sizeof (hash_t));
  h->u = apr_uniform_random_new ();
  h->size = next_prime_form_4k_plus_3 (HASH_TABLE_SIZE);

  h->p = MP_31;
  h->A = apr_uniform_random_next_int (&h->u);
  h->B = apr_uniform_random_next_int (&h->u);

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
apr_hash_table_destroy (hash_t *h)
{
  APR_FREE (h->entries);
  APR_FREE (h->status);
  APR_FREE (h);
}

APR_API unsigned long
apr_hash_str (hash_t *h, g_string s)
{
  long long hs = 0;
  uniform_random_t u;

  for (int i = 0; s[i] != '\0'; i++)
    {
      u = apr_uniform_random_new_with (s[i]);
      hs += s[i] * apr_uniform_random_next_int (&u);
    }

  hs &= h->p;
  return ((h->A * hs + h->B) & h->p) % h->size;
}

APR_API unsigned long
apr_hash_int (hash_t *h, g_int i)
{
  return ((h->A * i + h->B) & h->p) % h->size;
}

APR_API unsigned long
apr_hash_bool (hash_t *h, g_bool b)
{
  return ((h->A * ((int)b) + h->B) & h->p) % h->size;
}

APR_API unsigned long
apr_hash_float (hash_t *h, g_float f)
{
  long long int_val = (long long)(f * 1000000.0);

  return ((h->A * int_val + h->B) & h->p) % h->size;
}

APR_API unsigned long
_apr_hash (hash_t *h, g_entry_t *e)
{
  switch (e->type)
    {
    case GENTRY_INTEGER:
      return apr_hash_int (h, e->f_int);
      break;

    case GENTRY_BOOL:
      return apr_hash_bool (h, e->f_bool);
      break;

    case GENTRY_FLOAT:
      return apr_hash_float (h, e->f_float);
      break;

    case GENTRY_STRING:
      return apr_hash_str (h, e->f_string);
      break;

    default:
      eprintf ("invalid type '%d'", e->type);
      break;
    }

  return 0;
}
