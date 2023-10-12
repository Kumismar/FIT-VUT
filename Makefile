EXECUTABLE := dhcp-stats
CMDL_ARGS := -r juju -i jojo 172.181.11.0/1

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
	@rm -rvf $(BUILD) $(EXECUTABLE) $(TESTDIR)/build