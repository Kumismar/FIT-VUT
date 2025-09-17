EXECUTABLE = ipkcpd
CMDL_ARGS = -h 0.0.0.0 -p 2023 -m tcp

BUILD := build
TESTDIR := tests
SRCDIR := src

VPATH = $(SRCDIR)

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILD)/%.o, $(SRC))

CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -O3 

.PHONY: all run docs clean

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

docs:
	doxygen Doxyfile

clean:
	@rm -rvf $(BUILD) $(EXECUTABLE) $(TESTDIR)/build
