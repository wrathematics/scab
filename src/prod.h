#ifndef PROD_H
#define PROD_H


#include "types.h"

void pdgemm_(char *transa, char *transb, int *m, int *n, int *k, double *alpha, 
  double *a, int *ia, int *ja, int *desca, double *b, int *ib, int *jb, 
  int *descb, double *beta, double *c, int *ic, int *jc, int *descc);


// x^t * x
static inline void crossprod(double alpha, dmat_t *x, dmat_t *c)
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


#endif
