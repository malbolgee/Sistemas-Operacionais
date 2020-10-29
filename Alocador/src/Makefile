IDIR =../include
CC=g++
CFLAGS=-I -O2 -Wall -Wextra

ODIR=../obj

_DEPS = aloca.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o aloca.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~