#ifndef GRID_H
#define GRID_H


#include <mpi.h>

void Cblacs_exit(int NotDone);
void Cblacs_get(int ictxt, int what, int *val);
void Cblacs_gridexit(int Contxt);
void Cblacs_gridinfo(int ConTxt, int *nprow, int *npcol, int *myrow, int *mycol);
void Cblacs_gridinit(int *ConTxt, char *order, int nprow, int npcol);


static inline void grid_init(grid_t *g)
{
  char order = 'R';
  int size;
  
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  Cblacs_get(-1, 0, &(g->ictxt));
  Cblacs_gridinit(&(g->ictxt), &order, size, 1);
  Cblacs_gridinfo(g->ictxt, &(g->nprow), &(g->npcol), &(g->myrow), &(g->mycol));
}



static inline void grid_finalize(grid_t *g)
{
  Cblacs_gridexit(g->ictxt);
  Cblacs_exit(0);
}


#endif
