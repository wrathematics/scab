#include "errors.h"
#include "dmat.h"
#include "invert.h"
#include "grid.h"
#include "printer.h"
#include "prod.h"
#include "setup.h"
#include "timer.h"
#include "types.h"


// other parameters defined in setup.h
#define GRID_TYPE PROC_GRID_TALL

int main()
{
  dmat_t x;
  grid_t g;
  double t0;
  
  // int n = (int) sqrt((double) NUM_GPUS * GB_PER_GPU * BYTES_PER_VALUE);
  int n = 500;
  int nb = BLOCKSIZE;
  
  grid_init(&g, GRID_TYPE);
  
  t0 = get_time(0);
  dmat_init(&x, n, n, nb, nb, &g);
  dmat_fill_rand(&x, SEED);
  make_symmetric(&x);
  double t_gen = get_time(t0);
  
  t0 = get_time(0);
  invert(&x);
  double t_op = get_time(t0);
  
  MPI_print(&g, "invert,%d,%d,%d,%d,%d,%d,%d,%f,%f\n", g.size, g.nprow, g.npcol, n, n, nb, nb, t_gen, t_op);
  
  dmat_free(&x);
  grid_finalize(&g);
  
  return EXIT_OK;
}
