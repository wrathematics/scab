#ifndef TIMER_H
#define TIMER_H


#include <mpi.h>


static inline double get_time(double start)
{
  double t = MPI_Wtime() - start;
  MPI_Allreduce(MPI_IN_PLACE, &t, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  return t;
}


#endif
