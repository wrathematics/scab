suppressMessages(library(pbdDMAT))
source("setup.r")
init.grid(quiet=TRUE)
comm.set.seed(SEED, diff=TRUE)

n = 250
m = as.integer((NUM_GPUS * GB_PER_GPU) * (BYTES_PER_VALUE / n))

GRID_TYPE = PROC_GRID_TALL
.pbd_env$ICTXT = GRID_TYPE
.pbd_env$BLDIM = c(BLOCKSIZE, BLOCKSIZE)
g = base.blacs(GRID_TYPE)

t_gen = mpitime(x <- ddmatrix("runif", m, n))
t_op = mpitime(crossprod(x))

p = sprintf("crossprod,%d,%d,%d,%d,%d,%d,%d,%f,%f\n", comm.size(), g$NPROW, g$NPCOL, m, n, BLOCKSIZE, BLOCKSIZE, t_gen, t_op)
comm.cat(p, quiet=TRUE)

finalize()
