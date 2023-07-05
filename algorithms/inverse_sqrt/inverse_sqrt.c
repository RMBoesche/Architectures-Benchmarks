#include <stdio.h>
#include <time.h>

#define NUM 250000

float calcInvSqRoot( float n ) {

  const float threehalfs = 1.5F;
  float y = n;

  long i = * ( long * ) &y;

  i = 0x5f3759df - ( i >> 1 );
  y = * ( float * ) &i;

  y = y * ( threehalfs - ( (n * 0.5F) * y * y ) );

  return y;
}

int main() {
  int i = 0;
  int n = 1;
  clock_t t;
  double total_t = 0;
  double result = 0;

  t = clock();
  for (i=0;i<NUM;i++) {
    result += calcInvSqRoot(n);
    n = n*2;
    if (n > 81920)
      n = 1;
  }
  t = clock() - t;

  printf("Result: %f\n", result);
  total_t = ((double)t)/CLOCKS_PER_SEC;
  printf("TOTAL TIME: %.12fs\n", total_t);

  return 0;
}

