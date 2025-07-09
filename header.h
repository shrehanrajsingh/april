#if !defined(HEADER_H)
#define HEADER_H

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fwd.h"

#if !defined(APR_API)
#define APR_API
#endif // APR_API

#if !defined(eprintf)
#define eprintf(...) fprintf (stderr, __VA_ARGS__)
#endif // eprintf

#define G_SUCCESS 1
#define G_FAILURE 0

#define DBG(X) X

#endif // HEADER_H
