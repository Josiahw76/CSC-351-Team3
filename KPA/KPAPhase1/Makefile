# Author: Josiah Winiecki
#
# This makefile is responsible for building the Shell program

CWFLAGS = -Wall -Wextra 
SRCS := shellmain.o common.o shell.o

common.o:	common.h common.cpp
	g++ $(CWFLAGS) -c common.cpp

shell.o:	shell.h	shell.cpp
	g++ $(CWFLAGS) -c shell.cpp

shellmain.o:	shellmain.cpp
	g++ $(CWFLAGS) -c shellmain.cpp

shell:		$(SRCS)
	g++ $(SRCS) -o shell

clean:
	rm -f *.o shell *.exe
