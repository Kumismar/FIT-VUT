EXECUTABLE = ipkcpc
CMDL_ARGS = -m tcp -p 2023 -h 0.0.0.0

BUILD := build
TESTDIR := tests
SRCDIR := src

VPATH = $(SRCDIR)

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILD)/%.o, $(SRC))

CC := gcc
CXX := g++
CFLAGS := -std=c11 -Wall -Wextra -O3 #-Werror 

# I personally have googletest installed system-wide for Makefile simplification.
GTEST_LIB := /usr/local/lib64/libgtest.a # Specify your path here

TEST_SRC = $(TESTDIR)/cmd_args_tests.cpp # Specify the test here 
TEST_OBJ = $(filter-out $(BUILD)/ipkcpc.o, $(OBJ))

TEST_EXEC = $(BUILD)/test 

VALGRIND_OPTIONS := --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log

.PHONY: all run test runtest clean valgrind

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: %.c
	@[ -d $(BUILD) ] || mkdir $(BUILD)
	$(CC) $(CFLAGS) -c $^ -o $@

run: 
	@[ -f $(EXECUTABLE) ] || echo "The program executable doesn't exist. You can create it by running 'make'.\n"
	./$(EXECUTABLE) $(CMDL_ARGS)

test: 
	@cd tests && [ -d build/ ] || mkdir build/ && cd build/ && cmake .. && make -s

runtest: 
	./$(TESTDIR)/build/testos
	
valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(EXECUTABLE) $(CMDL_ARGS)

clean: 
	@rm -rvf $(BUILD) $(EXECUTABLE) $(TESTDIR)/$(BUILD)
