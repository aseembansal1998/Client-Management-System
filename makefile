#
## CSCI 370: Sample Makefile for OCCI compilation
#

G = g++-4.9
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei
all: main


        main.o: main.cpp
        $(G) -c $(CFLAGS) main.cpp


        main: main.o
        $(G) $(LFLAGS) -o exampleP main.o


        clean:
rm -f *.o main
