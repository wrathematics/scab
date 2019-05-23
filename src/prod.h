#ifndef PROD_H
#define PROD_H


#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "types.h"


extern void pdgemm_(char *transa, char *transb, int *m, int *n, int *k, double *alpha, 
  double *a, int *ia, int *ja, int *desca, double *b, int *ib, int *jb, 
  int *descb, double *beta, double *c, int *ic, int *jc, int *descc);

extern void pdgeadd_(char *trans, int *m, int *n, double *alpha, double *a, int *ia,
  int *ja, int *desca, double *beta, double *c, int *ic, int *jf, int *descc);

extern void pdsyrk_(char *uplo, char *trans, int *n, int *k, double *alpha, double *a,
  int *ia, int *ja, int *desca, double *beta, double *c, int *ic, int *jc,
  int *descc);



// x^t * x
static inline void crossprod_gemm(double alpha, dmat_t *x, dmat_t *c)
{
  char trans_t = 'T';
  char trans_n = 'N';
  int m = x->m;
  int n = x->n;
  int ij = 1;
  double zero = 0.0;
  
  pdgemm_(&trans_t, &trans_n, &n, &n, &m, &alpha, x->data, &ij, &ij, x->desc,
    x->data, &ij, &ij, x->desc, &zero, c->data, &ij, &ij, c->desc);
}



static inline void make_symmetric(dmat_t *x)
{
  #define INDXL2G(INDXLOC, NB, IPROC, NPROCS) (NPROCS*INDXLOC + (INDXLOC%NB) + NB*(NPROCS+IPROC)%NPROCS + 1) - 1
  
  double *x_data = x->data;
  int m = x->m;
  int n = x->n;
  int mb = x->mb;
  int nb = x->nb;
  int m_local = x->m_local;
  int n_local = x->n_local;
  
  if (m != n)
  MPI_error(x->g, EXIT_ERROR_NONCONFORMABLE, ERROR_NONCONFORMABLE_STRING);
  
  int nprow = x->g->nprow;
  int npcol = x->g->npcol;
  int myrow = x->g->myrow;
  int mycol = x->g->mycol;
  
  double *y = malloc(m_local*n_local * sizeof(*y));
  if (y == NULL)
    MPI_error(x->g, EXIT_ERROR_MALLOC, ERROR_MALLOC_STRING);
  
  // zero lower triangle of x
  for (int j=0; j<n_local; j++)
  {
    for (int i=0; i<m_local; i++)
    {
      int gi = INDXL2G(i, mb, myrow, nprow);
      int gj = INDXL2G(j, nb, mycol, npcol);
      int ind = i + m_local*j;
      if (gi > gj)
        x_data[ind] = 0.0;
      else if (gi == gj)
        x_data[ind] *= 0.5;
      
      y[ind] = x_data[ind];
    }
  }
  
  // x = x + y
  char trans = 'T';
  double s = 1.0;
  int ij = 1;
  pdgeadd_(&trans, &m, &n, &s, y, &ij, &ij, x->desc, &s, x_data, &ij, &ij, x->desc);
  
  free(y);
  #undef INDXL2G
}

// base/src/base/linalg/pcrossprod.f90
static inline void crossprod(double alpha, dmat_t *x, dmat_t *c)
{
  char trans = 'T';
  char uplo = 'U';
  int m = x->m;
  int n = x->n;
  int ij = 1;
  double zero = 0.0;
  
  pdsyrk_(&uplo, &trans, &n, &m, &alpha, x->data, &ij, &ij, x->desc, &zero,
    c->data, &ij, &ij, c->desc);
  
  make_symmetric(c);
}


#endif
