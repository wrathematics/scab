#ifndef INVERT_H
#define INVERT_H


#include <stdlib.h>

#include "errors.h"
#include "types.h"


void pdgetrf_(int *m, int *n, double *a, int *ia, int *ja, int *desca,
  int *ipiv, int *info);

void pdgetri_(int *n, double *a, int *ia, int *ja, int *desca, int *ipiv,
  double *work, int *lwork, int *iwork, int *liwork, int *info);


static inline void invert(dmat_t *x)
{
  int info;
  
  int n = x->n;
  int nb = x->nb;
  
  int ij = 1;
  
  // factor X = LU
  int *ipiv = malloc((n + nb) * sizeof(*ipiv));
  pdgetrf_(&n, &n, x->data, &ij, &ij, x->desc, ipiv, &info);
  if (info != 0)
    MPI_error(x->g, EXIT_ERROR_SCALAPACK, ERROR_SCALAPACK_STRING, "pdgetrf", info);
  
  // invert X
  double lwork_dbl;
  int lwork = -1;
  int liwork = -1;
  
  pdgetri_(&n, x->data, &ij, &ij, x->desc, ipiv, &lwork_dbl, &lwork, &liwork, &liwork, &info);
  lwork = (int) lwork_dbl;
  
  double *work = malloc(lwork * sizeof(*work));
  int *iwork = malloc(liwork * sizeof(*iwork));
  
  if (work == NULL || iwork == NULL)
    MPI_error(x->g, EXIT_ERROR_MALLOC, ERROR_MALLOC_STRING);
  
  pdgetri_(&n, x->data, &ij, &ij, x->desc, ipiv, work, &lwork, iwork, &liwork, &info);
  if (info != 0)
    MPI_error(x->g, EXIT_ERROR_SCALAPACK, ERROR_SCALAPACK_STRING, "pdgetri", info);
  
  free(work);
  free(ipiv);
}


#endif
