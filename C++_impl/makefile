

CXX = g++
VERSION = c++11
FLAGS = -g -Wall -Wno-unknown-pragmas -Wno-format-security

# OpenCL compile and link flags.
AOCL_COMPILE_CONFIG := $(shell aocl compile-config)
AOCL_LINK_CONFIG := $(shell aocl link-config)

SRC = sources/aux.cpp sources/triads.cpp sources/graph.cpp sources/edge.cpp



SEQ = exe/main_sequential

PAR = exe/main_parallel

RAND = exe/rand_graph_gen


all : $(SEQ) $(PAR) $(RAND)

sequential : $(SEQ)

parallel : $(PAR)

rand : $(RAND)

	
$(SEQ) : sources/main_sequential.cpp $(SRC)
	@echo "\n\033[31mCompiling $^\033[00m"
	@$(CXX) -std=$(VERSION) $(FLAGS) $^ -o $@ 
	@echo "\033[32mGenerated $@\033[00m"
	
$(PAR) : sources/main_parallel.cpp sources/aux_opencl.cpp $(SRC)
	@echo "\n\033[31mCompiling $^\033[00m"
	@$(CXX) -std=$(VERSION) $(FLAGS) $(AOCL_COMPILE_CONFIG) $^ $(AOCL_LINK_CONFIG) -o $@
	@rm -f *o
	@echo "\033[32mGenerated $@\033[00m"

$(RAND): sources/rand_graph_generation.c
	@echo "\n\033[31mCompiling $^\033[00m"
	@$(CC) -o $@ $^
	@echo "\033[32mGenerated $@\033[00m"

%.o: %.cpp
	$(CXX) -std=$(VERSION) -c $^ -o $@ 

clean_all : clean clean_graphs


clean :
	@rm -f *~ exe/*

clean_graphs : 
	@rm -f graphs/*txt
	
