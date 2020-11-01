CC=g++
ODIR=../obj
IDIR=../include
MKDIR_P= mkdir -p
CFLAGS=-I -O2 -Wall -Wextra

all: obj main

obj:
	${MKDIR_P} ../obj

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