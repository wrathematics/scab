PROC_GRID_SQUARE = 0
PROC_GRID_WIDE = 1
PROC_GRID_TALL = 2

mpitime = function(expr) pbdMPI::comm.timer(expr)[["max"]]

# -----------------------------------------------------------------------------
NUM_GPUS = 6
GB_PER_GPU = 12
BYTES_PER_VALUE = (1024*1024*1024/8)
BLOCKSIZE = 16
