MPICC = mpicc
CFLAGS = -O3 -std=c99 -D_XOPEN_SOURCE=600 -Wall -pedantic -Wextra
LDFLAGS = -lscalapack-openmpi -llapack -lblas -lm


all: clean crossprod

crossprod:
	$(MPICC) $(CFLAGS) src/crossprod.c -o crossprod $(LDFLAGS)

clean:
	rm -rf crossprod
