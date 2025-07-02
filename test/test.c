#include <april.h>

#define TEST(X) test##X ()

void
test1 ()
{
  graph_t *g = GRAPH ();
}

int
main (int argc, char const *argv[])
{
  printf ("Hello, from April!\n");
  return 0;
}