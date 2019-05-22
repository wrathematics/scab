#ifndef INVERT_H
#define INVERT_H


#include <stdlib.h>

#include "types.h"


static inline void invert(dmat_t *x)
{
  int info;
  
  int n = x->n;
  int nb = x->nb;
  
  int ij = 1;
  
  // factor X = LU
  int *ipiv = malloc((n + nb) * sizeof(*ipiv));
  pdgetrf_(&n, &n, x->data, &ij, &ij, x->desc, ipiv, &info);
  
  // invert X
  double lwork_dbl;
  int lwork = -1;
  int liwork = -1;
  
  pdgetri_(&n, x->data, &ij, &ij, x->desc, ipiv, &lwork_dbl, &lwork, &liwork, &liwork, &info);
  lwork = (int) lwork_dbl;
  
  double *work = malloc(lwork * sizeof(*work));
  int *iwork = malloc(liwork * sizeof(*iwork));
  
  pdgetri_(&n, x->data, &ij, &ij, x->desc, ipiv, work, &lwork, iwork, &liwork, &info);
  
  free(work);
  free(ipiv);
}


#endif
