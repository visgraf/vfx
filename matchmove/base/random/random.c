#include "random.h"


double uniform_random(void)
{
 return (double)rand()/(double) RAND_MAX;
}


void init_timer_random( void )
{
  srand(time(NULL));
}


