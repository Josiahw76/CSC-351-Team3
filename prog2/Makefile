CWFLAGS = -Wall -Wextra

all:		prog2

PQ.o:		pidPQ.h pidPQ.cpp
	g++ $(CWFLAGS) -O2 -c pidPQ.cpp

scheduler.o:	scheduler.h scheduler.cpp
	g++ $(CWFLAGS) -O2 -c scheduler.cpp

main.o:		main.cpp
	g++ $(CWFLAGS) -O2 -c main.cpp

prog2:		PQ.o scheduler.o main.o
	g++ $(CWFLAGS) -O2 PQ.o scheduler.o main.o -o

clean:
	rm -f *.o *.exe prog2
