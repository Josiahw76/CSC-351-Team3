# Work in progress.
# Type "make all" to get the executable

CFLAGS = -Wall -Wextra -O2

memory.o:	memory.h	memory.cpp
	g++ $(CFLAGS) -c memory.cpp -o memory.o

main.o:		main.cpp
	g++ $(CFLAGS) -c main.cpp -o main.o

memSim:		main.o	memory.o
	g++ $(CFLAGS) main.o memory.o -o memSim

all:	memSim


clean:	
	rm -f memSim memSim.exe *.o
