#include <stdio.h>
int func1(void);
int func2(void);
int
main (void)
{
  printf("%d\n", func1 ());
  printf("%d\n", func2 ());
  return 0;
}
