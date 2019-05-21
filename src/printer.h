#ifndef PRINTER_H
#define PRINTER_H


#include <stdarg.h>
#include <stdio.h>

#include "types.h"


void MPI_print(grid_t *g, const char *fmt, ...)
{
  if (g->myrow == 0 && g->mycol == 0)
  {
    va_list args;
    
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
}


#endif
