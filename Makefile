include args.txt 

EXECUTABLE := dhcp-stats
CMDL_ARGS := $(ARGS)

BUILD := build
TESTDIR := tests

.PHONY: all run docs clean

all: 
	@[ -d build ] || mkdir build
	@cd build && cmake .. && make

run:
	@[ -f $(EXECUTABLE) ] || echo "The program executable doesn't exist. You can create it by running 'make'.\n"
	./$(EXECUTABLE) $(CMDL_ARGS)

test: 
	@cd tests && [ -d build/ ] || mkdir build/ && cd build/ && cmake .. && make -s

runtest: 
	@[ -f $(TESTDIR)/build/testing ] || echo "The testing executable doesn't exist. You can create it by running 'make test'.\n"
	./$(TESTDIR)/build/testing

clean:
	@rm -rvf $(BUILD) $(EXECUTABLE) $(TESTDIR)/build valgrind.txt

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.txt ./$(EXECUTABLE) $(CMDL_ARGS)

man:
	man -l docs/dhcp-stats.1
