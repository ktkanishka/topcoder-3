include ../Makefile.common

OUTPUT_D = ../../build/haskell
SOLVER = $(OUTPUT_D)/FallingSandSolver
COMPILER = ghc -i$(GETTC_HOME)/include/haskell -outputdir $(OUTPUT_D)

$(SOLVER): FallingSandSolver.hs FallingSand.hs
	$(COMPILER) -o $(SOLVER) $<

main: setup $(SOLVER)

