#include <mpi.h>
#include <stdio.h>

#include "dmat.h"
#include "grid.h"
#include "prod.h"
#include "setup.h"
#include "types.h"


int main()
{
  dmat_t x, c;
  grid_t g;
  
  int n = 250;
  int m = (NUM_GPUS * GB_PER_GPU) * (BYTES_PER_VALUE / n);
  int mb = BLOCKSIZE, nb = BLOCKSIZE;
  
  grid_init(&g, GRID_TYPE);
  
  dmat_init(&x, m, n, mb, nb, g);
  dmat_init(&c, n, n, mb, nb, g);
  
  // dmat_print(&x);
  
  double t0 = MPI_Wtime();
  crossprod(1.0, &x, &c);
  double t1 = MPI_Wtime();
  
  if (g.myrow == 0 && g.mycol == 0)
  {
    // printf("Compute time: %.3f\n", t1-t0);
    printf("crossprod,%d,%d,%d,%d,%d,%f\n", g.nprow*g.npcol, m, n, mb, nb, t1-t0);
  }
  
  dmat_free(&x);
  dmat_free(&c);
  grid_finalize(&g);
  
  return 0;
}
