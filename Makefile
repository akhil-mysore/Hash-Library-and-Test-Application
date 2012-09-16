CC=gcc
CFLAGS=-I.
DEPS = hash_lib.h 
OBJ = hash_lib.c app.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

testhash : $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
