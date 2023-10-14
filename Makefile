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
	./$(TESTDIR)/build/testing

docs:
	doxygen Doxyfile

clean:
	@rm -rvf $(BUILD) $(EXECUTABLE) $(TESTDIR)/build valgrind.txt juju

valgrind: 
	valgrind --leak-check=full --track-origins=yes --log-file=valgrind.txt ./$(EXECUTABLE) $(CMDL_ARGS)