EXECUTABLE = simulation.out 
CMDL_ARGS = 

BUILD := build
SRCDIR := src

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILD)/%.o, $(SRC))

VPATH := $(SRCDIR)

CXX := g++
CXXFLAGS := -g -std=c++11 -Wall -Wextra  #-Werror 
LDLIBS = -lsimlib

VALGRIND_OPTIONS := --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log

.PHONY: all run clean valgrind doc

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD)/%.o: %.cpp
	@[ -d $(BUILD) ] || mkdir $(BUILD)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

run: 
	@[ -f $(EXECUTABLE) ] || echo "The program executable doesn't exist. You can create it by running 'make'.\n"
	./$(EXECUTABLE) $(CMDL_ARGS)

valgrind:
	valgrind $(VALGRIND_OPTIONS) ./$(EXECUTABLE) $(CMDL_ARGS)

clean: 
	@rm -rvf $(BUILD) $(EXECUTABLE) output.txt
	@cd doc/ && make clean

doc: 
	@cd doc/ && make
