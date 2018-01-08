
#makefile for GNU Linux

CC = gcc

FLAGS = -g -Wall -std=c99 -Wno-sign-compare -Wno-unknown-pragmas -Wno-format-security

# OpenCL compile and link flags.
AOCL_COMPILE_CONFIG := $(shell aocl compile-config)
AOCL_LINK_CONFIG := $(shell aocl link-config)

SRC = sources/aux.c sources/triads.c sources/graph.c sources/node.c sources/edge.c sources/helpers.c


SEQ = exe/main_sequential
PAR = exe/main_parallel

TIMES_SEQ = exe/times_sequential
TIMES_PAR = exe/times_parallel

RAND = exe/rand_graph_gen

sequential : $(SEQ)
parallel : $(PAR)

times_seq : $(TIMES_SEQ)
times_par : $(TIMES_PAR)

rand : $(RAND)


all : $(SEQ) $(PAR) $(TIMES_PAR) $(TIMES_SEQ) $(RAND)



$(SEQ) : sources/main_sequential.c $(SRC)
	@echo "\n\033[31mCompiling $^\033[00m"
	@$(CC) $(FLAGS) -o $@ $^
	@echo "\033[32mGenerated $@\033[00m"

$(TIMES_SEQ) : sources/times_sequential.c $(SRC)
	@echo "\n\033[31mCompiling $^\033[00m"
	@$(CC) $(FLAGS) -o $@ $^
	@echo "\033[32mGenerated $@\033[00m"

$(PAR) : sources/main_parallel.c sources/aux_opencl.c $(SRC)
	@echo "\n\033[31mCompiling $^\033[00m"
	@$(CC) $(FLAGS) $(AOCL_COMPILE_CONFIG) $^ $(AOCL_LINK_CONFIG) -o $@
	@rm -f *o
	@echo "\033[32mGenerated $@\033[00m"


$(TIMES_PAR) : sources/times_parallel.c sources/aux_opencl.c $(SRC)
	@echo "\n\033[31mCompiling $^\033[00m"
	@$(CC) $(FLAGS) $(AOCL_COMPILE_CONFIG) $^ $(AOCL_LINK_CONFIG) -o $@
	@rm -f *o
	@echo "\033[32mGenerated $@\033[00m"


$(RAND): sources/rand_graph_generation.c
	@echo "\n\033[31mCompiling $^\033[00m"
	@$(CC) -o $@ $^
	@echo "\033[32mGenerated $@\033[00m"


clean_all : clean clean_graphs clean_times


clean :
	@rm -f *~ exe/*


clean_graphs : 
	@rm -f graphs/*txt

clean_times :
	@rm -f times/*txt

.PHONY : clean
