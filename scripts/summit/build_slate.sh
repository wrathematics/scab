#!/bin/sh

# download slate
module load mercurial
hg clone https://bitbucket.org/icl/slate
cd slate


# build slate
module swap xl gcc/6.4.0
module load cuda
module load essl
module load netlib-lapack/3.8.0
module load netlib-scalapack/2.0.2
module load cmake

echo '
CXX=mpicxx

spectrum=1
essl=1

cuda=1
openmp=1
cuda_arch="volta"
' > make.inc

export LIBRARY_PATH=$LD_LIBRARY_PATH
make -j 16
