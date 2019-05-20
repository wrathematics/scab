#ifndef TYPES_H
#define TYPES_H


typedef struct grid_t
{
  int ictxt;
  int nprow;
  int npcol;
  int myrow;
  int mycol;
} grid_t;



typedef struct dmat_t
{
  int m;
  int n;
  int m_local;
  int n_local;
  int mb;
  int nb;
  double *data;
  int desc[9];
} dmat_t;


#endif
