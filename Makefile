.POSIX:
CROSS	= 
CC = $(CROSS)gcc
COMPILER_WARNING_FLAGS = -Wall -Wextra -Wconversion -Wdouble-promotion -Wnull-dereference -Wwrite-strings -Wformat=2 -Wcast-align=strict -Wswitch-enum -Wpedantic
# COMPILER_SANITIZERS =
COMPILER_SANITIZERS = -fsanitize=bool -fsanitize=undefined -fsanitize-trap
DEFAULT_COMPILER_OPTIMIZATION_LEVEL = -Og -g3
COMPILER_OPTIMIZATION_LEVEL_SPEED = -O3
COMPILER_OPTIMIZATION_LEVEL_SPACE = -Os
COMPILER_STANDARD = -std=c99
INCLUDE_PATHS = -I. -Idep/Unity/src
CFLAGS = $(INCLUDE_PATHS) $(COMPILER_WARNING_FLAGS) $(COMPILER_SANITIZERS) $(DEFAULT_COMPILER_OPTIMIZATION_LEVEL) $(COMPILER_STANDARD)
CFLAGS_TEST = $(COMPILER_WARNING_FLAGS) $(COMPILER_SANITIZERS) $(DEFAULT_COMPILER_OPTIMIZATION_LEVEL) $(COMPILER_STANDARD) -DTEST
CFLAGS_FAST = $(COMPILER_WARNING_FLAGS) $(COMPILER_OPTIMIZATION_LEVEL_SPEED)
CFLAGS_SMALL = $(COMPILER_WARNING_FLAGS) $(COMPILER_OPTIMIZATION_LEVEL_SPACE)
LDFLAGS = 

# Rules for building the executable, object file, and listing file

# TODO - Unit test target
PATH_UNITY = dep/Unity/
PATH_TEST = test/
# TODO - Fuzz testing
# TODO - Benchmark target

.PHONY: test
test: test_lin_pid.exe
	@echo
	@echo "----------------------------------------"
	@echo "Running $<..."
	@echo
	./$< > test/test_lin_pid.txt 2>&1
	@echo

test_lin_pid.exe: test_lin_pid.o lin_pid.o
	@echo
	@echo "----------------------------------------"
	@echo "Linking $^ into executable..."
	@echo
	$(CC) $(LDFLAGS) $^ -o $@

test_lin_pid.o: test_lin_pid.c
	@echo
	@echo "----------------------------------------"
	@echo "Compiling $<..."
	@echo
	$(CC) -c $(CFLAGS_TEST) $< -o $@

# Run it, baby!
.PHONY: run
run: lin_pid.exe lin_pid.lst	# Don't actually need the .lst file but want to force the disassembly generation
	@echo
	@echo "----------------------------------------"
	@echo "Running $<..."
	@echo
	./$<
	@echo

# Produces an object dump that includes the disassembly of the executable
lin_pid.lst: lin_pid.exe
	@echo
	@echo "----------------------------------------"
	@echo "Disassembly of $<..."
	@echo
	objdump -D $< > $@

# Build the executable
lin_pid.exe: lin_pid.o
	@echo
	@echo "----------------------------------------"
	@echo "Linking $< into executable..."
	@echo
	$(CC) $(LDFLAGS) $< -o $@

# Build the object file and run static analysis against it
lin_pid.o: lin_pid.c
	@echo
	@echo "----------------------------------------"
	@echo "Compiling $<..."
	@echo
	$(CC) -c $(CFLAGS) $< -o $@
	@echo
	@echo "----------------------------------------"
	@echo "Running static analysis on $<..."
	@echo
	cppcheck --template='{severity}: {file}:{line}: {message}' $< 2>&1 | tee cppcheck.log | python colorize_cppcheck.py

# Clean rule to remove generated files
.PHONY: clean
clean:
	rm -f *.exe *.o *.lst
