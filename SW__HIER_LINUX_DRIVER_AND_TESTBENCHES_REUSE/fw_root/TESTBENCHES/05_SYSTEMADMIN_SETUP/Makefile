CC = gcc
CFLAGS = -c -fPIC
SHARED_FLAGS = -shared -Wl,-soname,libkeccak.so.1
CFLAGS_EXEC = -I../../LINUX_KERNEL_MODULE -I.

all: clean keccak.o libkeccak.so run.x

keccak.o: keccak.c keccak.h
	$(CC) $(CFLAGS) keccak.c

libkeccak.so: keccak.o
	$(CC) $(SHARED_FLAGS) -o libkeccak.so.1.0 keccak.o
	mv libkeccak.so.1.0 /usr/local/lib
	ln -s /usr/local/lib/libkeccak.so.1.0 /usr/local/lib/libkeccak.so.1
	ln -s /usr/local/lib/libkeccak.so.1.0 /usr/local/lib/libkeccak.so

run.x: systemadmin_setup.c libkeccak.so
	$(CC) -I../../LINUX_KERNEL_MODULE -I.  -L/usr/local/lib -o run.x systemadmin_setup.c -lkeccak
	ldconfig

clean:
	rm -f *.o *.so systemadmin_setup
	rm -f /usr/local/lib/libkeccak.*
