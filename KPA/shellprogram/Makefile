# Author: Josiah Winiecki
#
# This makefile is responsible for building the Shell program

CWFLAGS = -Wall -Wextra 
SRCS := main.o kpaTCP.o shell.o

kpaTCP.o:	kpaTCP.h kpaTCP.cpp
	g++ $(CWFLAGS) -c kpaTCP.cpp

shell.o:	shell.h	shell.cpp
	g++ $(CWFLAGS) -c shell.cpp

main.o:		main.cpp
	g++ $(CWFLAGS) -c main.cpp

shell:		$(SRCS)
	g++ $(SRCS) -o shell

servertest.o:	servertest.cpp
	g++ $(CWFLAGS) -c servertest.cpp

server:		servertest.o kpaTCP.o
	g++ $(CWFLAGS) servertest.o kpaTCP.o -o server


clean:
	rm -f *.o shell server *.exe
