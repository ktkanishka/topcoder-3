include ../Makefile.common

OUTPUT_D =../../build/cpp
SOLVER = $(OUTPUT_D)/MutaliskEasySolver
COMPILER = g++ -std=gnu++0x -I$(GETTC_HOME)/include/cpp

$(SOLVER): MutaliskEasySolver.cpp MutaliskEasy.cpp
	$(COMPILER) -o $(SOLVER) $<

main: setup $(SOLVER)
