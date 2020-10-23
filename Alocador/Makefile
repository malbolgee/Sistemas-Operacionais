CC=g++
CFLGAS=-I -O2 -Wall -Wextra
OBJ = main.o aloca.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLGAS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLGAS)

clean:
	rm *.o