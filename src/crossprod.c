#include <mpi.h>
#include <stdio.h>

#include "dmat.h"
#include "grid.h"
#include "prod.h"
#include "setup.h"
#include "types.h"


static inline double get_time(double start)
{
  return MPI_Wtime() - start;
}

// parameters defined in setup.h

int main()
{
  dmat_t x, c;
  grid_t g;
  double t0;
  
  int n = 250;
  int m = (NUM_GPUS * GB_PER_GPU) * (BYTES_PER_VALUE / n);
  int mb = BLOCKSIZE, nb = BLOCKSIZE;
  
  grid_init(&g, GRID_TYPE);
  
  t0 = get_time(0);
  dmat_init(&x, m, n, mb, nb, g);
  double t_gen = get_time(t0);
  dmat_init(&c, n, n, mb, nb, g);
  
  t0 = get_time(0);
  crossprod(1.0, &x, &c);
  double t_op = get_time(t0);
  
  if (g.myrow == 0 && g.mycol == 0)
  {
    // printf("Compute time: %.3f\n", t1-t0);
    printf("crossprod,%d,%d,%d,%d,%d,%d,%d,%f,%f\n", 1, g.nprow, g.npcol, m, n, mb, nb, t_gen, t_op);
  }
  
  dmat_free(&x);
  dmat_free(&c);
  grid_finalize(&g);
  
  return 0;
}
