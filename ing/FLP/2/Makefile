PROLOG_SRC = ntm_sim.pl
TARGET_EXEC = flp24-log
COMPILE_FLAGS = --goal=main --stack_limit=16g

.PHONY: all clean

all: $(TARGET_EXEC)

$(TARGET_EXEC): $(PROLOG_SRC)
	@swipl $(COMPILE_FLAGS) -o $@ -c $<

clean:
	@rm -f $(TARGET_EXEC)