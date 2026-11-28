# Author: Josiah Winiecki
#
# This makefile is responsible for building the FS program

CWFLAGS = -std=c++17 -Wall -Wextra
SRCS = common.o blocks.o directory.o fdTable.o kpaFScommands.o KPAsocket.o kpaFSmain.o

.PHONY: all shell clean
all: kpa shell

shell:
	$(MAKE) -C shellprogram shell CWFLAGS="$(CWFLAGS)"

common.o:	common.h common.cpp
	g++ $(CWFLAGS) -c common.cpp

#FS COMPILATION

blocks.o:	blocks.cpp blocks.h iNode.h
	g++ $(CWFLAGS) -c blocks.cpp

directory.o:	directory.cpp directory.h blocks.h
	g++ $(CWFLAGS) -c directory.cpp

fdTable.o:	fdTable.cpp fdTable.h
	g++ $(CWFLAGS) -c fdTable.cpp

kpaFScommands.o:	kpaFScommands.cpp kpaFScommands.h blocks.h iNode.h fdTable.h
	g++ $(CWFLAGS) -c kpaFScommands.cpp

KPAsocket.o:	KPAsocket.cpp KPAsocket.h common.h kpaFScommands.h blocks.h iNode.h fdTable.h
	g++ $(CWFLAGS) -c KPAsocket.cpp

kpaFSmain.o:	kpaFSmain.cpp KPAsocket.h common.h kpaFScommands.h blocks.h iNode.h fdTable.h
	g++ $(CWFLAGS) -c kpaFSmain.cpp

kpa:	$(SRCS)
	g++ $(CWFLAGS) $(SRCS) -o kpa

clean:
	rm -f *.o *.exe kpa
	$(MAKE) -C shellprogram clean