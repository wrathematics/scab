#include "errors.h"
#include "dmat.h"
#include "grid.h"
#include "printer.h"
#include "prod.h"
#include "setup.h"
#include "timer.h"
#include "types.h"


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
  dmat_init(&x, m, n, mb, nb, &g);
  dmat_fill_rand(&x);
  double t_gen = get_time(t0);
  
  t0 = get_time(0);
  dmat_init(&c, n, n, mb, nb, &g);
  crossprod(1.0, &x, &c);
  double t_op = get_time(t0);
  
  MPI_print(&g, "crossprod,%d,%d,%d,%d,%d,%d,%d,%f,%f\n", g.size, g.nprow, g.npcol, m, n, mb, nb, t_gen, t_op);
  
  dmat_free(&x);
  dmat_free(&c);
  grid_finalize(&g);
  
  return EXIT_OK;
}
