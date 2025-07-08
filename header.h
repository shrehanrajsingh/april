#if !defined(HEADER_H)
#define HEADER_H

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fwd.h"

#if !defined(APR_API)
#define APR_API
#endif // APR_API

#if !defined(eprintf)
#define eprintf(...) fprintf (stderr, __VA_ARGS__)
#endif // eprintf

#endif // HEADER_H
