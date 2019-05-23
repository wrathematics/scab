#ifndef ERRORS_H
#define ERRORS_H



#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"


#define EXIT_OK 0

#define EXIT_ERROR_MALLOC -1
#define ERROR_MALLOC_STRING "ERROR: malloc failed\n"

#define EXIT_ERROR_NONCONFORMABLE -2
#define ERROR_NONCONFORMABLE_STRING "ERROR: operation on non-conformable arrays"

#define EXIT_ERROR_SCALAPACK -3
#define ERROR_SCALAPACK_STRING "ERROR: ScaLAPACK function %s failed with info=%d"


static inline void MPI_error(grid_t *g, int code, const char *fmt, ...)
{
  if (g->myrow == 0 && g->mycol == 0)
  {
    va_list args;
    
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
  
  exit(code);
}


#endif
