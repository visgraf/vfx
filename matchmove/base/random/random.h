#ifndef RANDOM_H
#define RANDOM_H


#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define RND() (uniform_random())


void init_timer_random( void );
double uniform_random( void );

#endif
