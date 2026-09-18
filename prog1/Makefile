# Josiah
# make win for windows, make mac for mac.

CFLAGS = -Wall -Wextra -O2

memory.o:	memory.h	memory.cpp
	g++ $(CFLAGS) -c memory.cpp -o memory.o

main.o:		main.cpp
	g++ $(CFLAGS) -c main.cpp -o main.o

mac:		main.o	memory.o
	g++ $(CFLAGS) main.o memory.o -o memSim

win:		main.o	memory.o
	g++ $(CFLAGS) main.o memory.o -o memSim.exe

all:	memSim


clean:	
	rm -f memSim memSim.exe *.o
