#ifndef DMAT_H
#define DMAT_H


#include <string.h>

#include "errors.h"
#include "rand.h"
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
  int m_local = numroc(m, mb, g->myrow, 0, g->nprow);
  int n_local = numroc(n, nb, g->mycol, 0, g->npcol);
  
  double *data = malloc(m_local*n_local * sizeof(*data));
  if (data == NULL)
    MPI_error(g, EXIT_ERROR_MALLOC, ERROR_MALLOC_STRING);
  
  descinit(x->desc, g->ictxt, m, n, mb, nb, m_local);
  
  x->m = m;
  x->n = n;
  x->m_local = m_local;
  x->n_local = n_local;
  x->mb = mb;
  x->nb = nb;
  x->data = data;
  x->g = g;
  
  #undef DATA
  #undef LOCM
  #undef LOCN
}



static inline void dmat_free(dmat_t *x)
{
  free(x->data);
}



static inline void dmat_fill_rand(dmat_t *x, int seed)
{
  #define DATA(x) (x->data)
  #define LOCM(x) (x->m_local)
  #define LOCN(x) (x->n_local)
  
  rand_init(seed);
  
  for (int j=0; j<LOCN(x); j++)
  {
    for (int i=0; i<LOCM(x); i++)
      DATA(x)[i + LOCM(x)*j] = rand_unif(0, 1);
  }
  
  #undef DATA
  #undef LOCM
  #undef LOCN
}



static inline void dmat_fill_zero(dmat_t *x)
{
  memset(x->data, 0, x->m_local*x->n_local*sizeof(*(x->data)));
}



// DATA(y) := DATA(x)
static inline void dmat_copy(dmat_t *x, dmat_t *y)
{
  #define DATA(x) (x->data)
  #define LOCM(x) (x->m_local)
  #define LOCN(x) (x->n_local)
  
  if (x->m != y->m || x->n != y->n || x->mb != y->mb || x->nb != y->nb || x->desc[1] != y->desc[1] || x->g != y->g)
    MPI_error(x->g, EXIT_ERROR_NONCONFORMABLE, ERROR_NONCONFORMABLE_STRING);
  
  for (int j=0; j<LOCN(x); j++)
  {
    for (int i=0; i<LOCM(x); i++)
    {
      int ind = i + LOCM(x)*j;
      DATA(x)[ind] = DATA(y)[ind];
    }
  }
  
  #undef DATA
  #undef LOCM
  #undef LOCN
}


#endif
