CC=g++
CFLGAS=-I -O2 -Wall -Wextra
OBJ = main.o aloca.o
DEBUGF=_

%.o: %.cpp
	$(CC) -D ${DEBUGF} -c -o $@ $< $(CFLGAS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLGAS)

clean:
	rm *.o