CC=gcc
ODIR=../obj
IDIR =../include
MKDIR_P=mkdir -p
CFLAGS=-I -O -Wall -lpthread -Wextra -Werror

all: obj rlezipm rlezips

obj:
	${MKDIR_P} ../obj

_DEPS = utils.h threadhandler.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = rlezipm.o utils.o threadhandler.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ1 = rlezips.o utils.o
OBJ1 = $(patsubst %,$(ODIR)/%,$(_OBJ1))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rlezipm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

rlezips: $(OBJ1)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~