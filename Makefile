EXECUTABLE := dhcp-stats
CMDL_ARGS := -r juju -i jojo 172.181.11.0/1

BUILD := build
TESTDIR := tests
SRCDIR := src

VPATH = $(SRCDIR)

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILD)/%.o, $(SRC))

CC := g++
CFLAGS := -std=c++20 -Wall -Wextra -O3 

.PHONY: all run docs clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: %.cpp
	@[ -d $(BUILD) ] || mkdir $(BUILD)
	$(CC) $(CFLAGS) -c -g $^ -o $@

run:
	@[ -f $(EXECUTABLE) ] || echo "The program executable doesn't exist. You can create it by running 'make'.\n"
	./$(EXECUTABLE) $(CMDL_ARGS)

test: 
	@cd tests && [ -d build/ ] || mkdir build/ && cd build/ && cmake .. && make -s

runtest: 
	./$(TESTDIR)/build/testos

docs:
	doxygen Doxyfile

clean:
	@rm -rvf $(BUILD) $(EXECUTABLE) $(TESTDIR)/build