#ifndef GRID_H
#define GRID_H


#include <math.h>
#include <mpi.h>

#define PROC_GRID_SQUARE  0
#define PROC_GRID_WIDE    1
#define PROC_GRID_TALL    2


extern void Cblacs_exit(int NotDone);
extern void Cblacs_get(int ictxt, int what, int *val);
extern void Cblacs_gridexit(int Contxt);
extern void Cblacs_gridinfo(int ConTxt, int *nprow, int *npcol, int *myrow, int *mycol);
extern void Cblacs_gridinit(int *ConTxt, char *order, int nprow, int npcol);



static inline void grid_init(grid_t *g, int gridtype)
{
  char order = 'R';
  int size;
  int ictxt;
  
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  Cblacs_get(-1, 0, &ictxt);
  
  if (gridtype == PROC_GRID_SQUARE)
  {
    int nr, nc;
    int n = (int) sqrt((double) size);
    n = (n<1)?1:n;
    
    for (int i=0; i<n; i++)
    {
      nc = n - i;
      nr = size % nc;
      if (nr == 0)
        break;
    }
    
    nr = size / nc;
    
    Cblacs_gridinit(&ictxt, &order, nr, nc);
  }
  else if (gridtype == PROC_GRID_TALL)
    Cblacs_gridinit(&ictxt, &order, size, 1);
  else if (gridtype == PROC_GRID_WIDE)
    Cblacs_gridinit(&ictxt, &order, 1, size);
  else
  {
    fprintf(stderr, "ERROR: impossible grid type");
    exit(-1);
  }
  
  Cblacs_gridinfo(ictxt, &(g->nprow), &(g->npcol), &(g->myrow), &(g->mycol));
  g->ictxt = ictxt;
  g->size = size;
}



static inline void grid_finalize(grid_t *g)
{
  int mpi_continue = 0;
  Cblacs_gridexit(g->ictxt);
  Cblacs_exit(mpi_continue);
}


#endif
