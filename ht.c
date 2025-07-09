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

  for (int i = 0; i < h->size; i++)
    h->entries[i] = NULL;

  memset (h->status, INACTIVE, h->size * sizeof (enum HashStatusEnum));

  h->f1 = &crs1;
  h->f2 = &crs2;

  return h;
}

APR_API hash_t *
apr_hash_table_new_with (size_t s, uniform_random_t u, int p)
{
  hash_t *h = APR_MALLOC (sizeof (hash_t));
  h->u = u;
  h->size = next_prime_form_4k_plus_3 (s);

  h->p = p;
  h->A = apr_uniform_random_next_int (&h->u);
  h->B = apr_uniform_random_next_int (&h->u);

  h->entries = APR_MALLOC (h->size * sizeof (*h->entries));
  h->status = APR_MALLOC (h->size * sizeof (*h->status));

  for (int i = 0; i < h->size; i++)
    h->entries[i] = NULL;

  memset (h->status, INACTIVE, h->size * sizeof (enum HashStatusEnum));

  h->f1 = &crs1;
  h->f2 = &crs2;

  return h;
}

static size_t
next_prime_form_4k_plus_3 (size_t m)
{
  size_t k = ((m - 3) >> 2) + 1;
  size_t p = (k << 2) + 3;

  int is_prime;
l1:
  is_prime = 1;
  for (size_t i = 2; is_prime && i * i <= p; ++i)
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
  for (int i = 0; i < h->size; i++)
    {
      if (h->entries[i] != NULL)
        APR_FREE (h->entries[i]);
    }

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

  return G_FAILURE;
}

APR_API int
apr_hash_add_key (hash_t *h, g_entry_t *k, void *v)
{
  int got_key = 0;
  size_t kh = _apr_hash (h, k);
  // printf ("kh: %ld\n", kh);
  size_t *hashst = APR_MALLOC (32 * sizeof (size_t));

  size_t hstc = 32;
  size_t hstl = 0;

  hashst[hstl++] = kh;

  int i = 0;
  while (hstl) /* consider only INACTIVE and DELETED entries */
    {
      size_t p = hashst[--hstl];

      if (h->status[p] != ACTIVE)
        {
          got_key = 1;
          kh = p;
          break;
        }

      if (p == kh && hstl != 0) /* entire table is full */
        {
          // eprintf ("table full, key not inserted\n");
          DBG (printf ("table full, resizing...\n"));
          apr_hash_resize (&h);
        }

      size_t kh1 = (p + h->f1 (i)) % h->size;
      size_t kh2 = (p + h->f2 (i)) % h->size;

      if (hstl + 2 >= hstc)
        {
          hstc += 32;
          hashst = APR_REALLOC (hashst, hstc * sizeof (size_t));
        }

      hashst[hstl++] = kh1;
      hashst[hstl++] = kh2;
      i++;
    }
  APR_FREE (hashst);

  /* kh is the required key */
  h->status[kh] = ACTIVE;

  h->entries[kh] = APR_MALLOC (sizeof (**h->entries));
  h->entries[kh]->key = k;
  h->entries[kh]->val = v;

  return G_SUCCESS;
}

APR_API void *
apr_hash_get (hash_t *h, g_entry_t *k, int *f)
{
  int got_key = 0;
  size_t kh = _apr_hash (h, k);
  size_t *hashst = APR_MALLOC (32 * sizeof (size_t));

  size_t hstc = 32;
  size_t hstl = 0;

  hashst[hstl++] = kh;

  int i = 0;
  while (hstl) /* consider only INACTIVE and DELETED entries */
    {
      size_t p = hashst[--hstl];

      if (h->status[p] == ACTIVE)
        {
          got_key = 1;
          kh = p;
          break;
        }

      if (p == kh && !hstl) /* key not found */
        break;

      size_t kh1 = (p + h->f1 (i)) % h->size;
      size_t kh2 = (p + h->f2 (i)) % h->size;

      if (hstl + 2 >= hstc)
        {
          hstc += 32;
          hashst = APR_REALLOC (hashst, hstc * sizeof (size_t));
        }

      hashst[hstl++] = kh1;
      hashst[hstl++] = kh2;
    }
  APR_FREE (hashst);

  if (f != NULL)
    *f = got_key;

  return h->entries[kh]->val;
}

APR_API void
apr_hash_resize (hash_t **href)
{
  hash_t *old = *href;
  hash_t *nt = apr_hash_table_new_with (old->size * 2,
                                        apr_uniform_random_new (), old->p);

  if (nt == NULL)
    {
      eprintf ("apr_hash_resize(): memory allocation failed, exiting...");
      exit (EXIT_FAILURE);
    }

  for (int i = 0; i < old->size; i++)
    {
      enum HashStatusEnum e = old->status[i];

      if (e == ACTIVE)
        apr_hash_add_key (nt, old->entries[i]->key, old->entries[i]->val);
    }

  apr_hash_table_destroy (old);
  *href = nt;
}