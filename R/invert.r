suppressMessages(library(pbdDMAT))
source("setup.r")
init.grid(quiet=TRUE)
comm.set.seed(SEED, diff=TRUE)

n = 5000

GRID_TYPE = PROC_GRID_SQUARE
.pbd_env$ICTXT = GRID_TYPE
.pbd_env$BLDIM = c(BLOCKSIZE, BLOCKSIZE)
g = base.blacs(GRID_TYPE)

t_gen = mpitime({
  x <- ddmatrix("runif", n, n)
  x <- crossprod(x)
})
t_op = mpitime(solve(x))

p = sprintf("invert,%d,%d,%d,%d,%d,%d,%d,%f,%f\n", comm.size(), g$NPROW, g$NPCOL, n, n, BLOCKSIZE, BLOCKSIZE, t_gen, t_op)
comm.cat(p, quiet=TRUE)

finalize()
