#ifndef DMAT_H
#define DMAT_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "errors.h"
#include "types.h"


static inline void descinit(int *desc, int ictxt, int m, int n, int mb, int nb, int lld)
{
  desc[0] = 1;
  desc[1] = ictxt;
  desc[2] = m;
  desc[3] = n;
  desc[4] = mb;
  desc[5] = nb;
  desc[6] = 0;
  desc[7] = 0;
  desc[8] = lld;
}



static inline int numroc(int n, int nb, int iproc, int isrcproc, int nprocs)
{
  int mydist = (nprocs+iproc-isrcproc) % nprocs;
  int nblocks = n / nb;
  
  int ret = (nblocks/nprocs) * nb;
  
  int extrablks = nblocks % nprocs;
  if (mydist < extrablks)
    ret += nb;
  else if (mydist == extrablks)
    ret += (n%nb);
  
  return ret;
}



static inline void dmat_init(dmat_t *x, int m, int n, int mb, int nb, grid_t *g)
{
  #define DATA(x) (x->data)
  #define LOCM(x) (x->m_local)
  #define LOCN(x) (x->n_local)
  
  x->m = m;
  x->n = n;
  
  LOCM(x) = numroc(m, mb, g->myrow, 0, g->nprow);
  LOCN(x) = numroc(n, nb, g->myrow, 0, g->npcol);
  
  DATA(x) = malloc(LOCM(x)*LOCN(x) * sizeof(*DATA(x)));
  
  if (DATA(x) == NULL)
    MPI_error(g, EXIT_ERROR_MALLOC, ERROR_MALLOC_STRING);
  
  descinit(x->desc, g->ictxt, m, n, mb, nb, x->m_local);
  
  x->g = g;
  
  #undef DATA
  #undef LOCM
  #undef LOCN
}



static inline void dmat_fill_rand(dmat_t *x)
{
  #define DATA(x) (x->data)
  #define LOCM(x) (x->m_local)
  #define LOCN(x) (x->n_local)
  
  srandom(time(NULL) ^ getpid());
  for (int j=0; j<LOCN(x); j++)
  {
    for (int i=0; i<LOCM(x); i++)
      DATA(x)[i + LOCM(x)*j] = ((double) random() / ((double) RAND_MAX+1));
  }
  
  #undef DATA
  #undef LOCM
  #undef LOCN
}



static inline void dmat_free(dmat_t *x)
{
  free(x->data);
}


#endif
