# Author: Josiah Winiecki
#
# This makefile is responsible for building the Shell program

CWFLAGS = -Wall -Wextra 
SRCS := main.o
LFLAGS := 


main.o:		main.cpp
	g++ $(CWFLAGS) -c main.cpp

shell:		$(SRCS)
	g++ $(SRCS) -o shell $(LFLAGS)

server:		tcpserver.cpp
	g++ $(CWFLAGS) tcpserver.cpp -o server

both:		shell server

clean:
	rm -f *.o shell server *.exe
