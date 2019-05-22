MPICC = mpicc
CFLAGS = -O3 -std=c99 -D_XOPEN_SOURCE=600 -Wall -pedantic -Wextra
LDFLAGS = -lscalapack-openmpi -llapack -lblas -lm


all: clean crossprod invert

crossprod:
	$(MPICC) $(CFLAGS) src/crossprod.c -o crossprod $(LDFLAGS)

invert:
	$(MPICC) $(CFLAGS) src/invert.c -o invert $(LDFLAGS)

clean:
	rm -rf crossprod invert
