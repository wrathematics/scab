#ifndef TIMER_H
#define TIMER_H


#include <mpi.h>

static inline double get_time(double start)
{
  return MPI_Wtime() - start;
}


#endif
