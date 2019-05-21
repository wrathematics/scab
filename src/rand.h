#ifndef RAND_H
#define RAND_H


#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


static inline void rand_init(int seed)
{
  srandom(seed ^ getpid());
}



static inline double rand_unif(const double min, const double max)
{
  return min + (max - min + 1.0) * ((double) random() / ((double) RAND_MAX+1));
}


#endif
