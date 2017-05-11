###########################################################################
##                                                                       ##
## Project: C++ csvfile library.                                             ##
##                                                                       ##
## Description: Build csvfile library                                        ##
##                                                                       ##
## Date: May 10, 2017.                                                 ##
##                                                                       ##
###########################################################################

##
## $Id$
##
## $Log$

.SUFFIXES:
.SUFFIXES: .c .cpp .o .y

MAKEFILE_DEPEND = .depend

LINK=g++ $^ -o $@

.cpp.o:
	g++ -std=c++11 -Wall -Wextra -I. -c $< -o $@

TARGETS =
TARGETS += csvfile_main

all:	$(TARGETS)

test:
	make -C ./tests
	./tests/runTests

depend:
	g++ -MM *.cpp > $(MAKEFILE_DEPEND)

clean:
	rm -rf *.o $(TARGETS) $(MAKEFILE_DEPEND) html latex

doc:
	doxygen -g
	doxygen

csvfile_main: csvfile_main.o csvfile.o
	$(LINK)

-include $(MAKEFILE_DEPEND)

## End of file 

	
