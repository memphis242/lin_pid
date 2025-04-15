.POSIX:

## The Prelude

CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION=exe
# Set the OS-specific tool cmds / executable extensions
# ifeq ($(OS),Windows_NT)
#   ifeq ($(shell uname -s),) # not in a bash-like shell
# 	CLEANUP = del /F /Q
# 	MKDIR = mkdir
#   else # in a bash-like shell, like msys
# 	CLEANUP = rm -f
# 	MKDIR = mkdir -p
#   endif
# else
# 	CLEANUP = rm -f
# 	MKDIR = mkdir -p
# 	TARGET_EXTENSION=out
# endif

# Relevant paths
PATH_UNITY        = Unity/src/
PATH_SRC          = src/
PATH_TEST_FILES   = test/
PATH_BUILD        = build/
PATH_OBJECT_FILES = build/objs/
PATH_RESULTS      = build/results/

# List of all the build paths
BUILD_PATHS = $(PATH_BUILD) $(PATH_OBJECT_FILES) $(PATH_RESULTS)
# List of all the test .c files
SRC_TEST_FILES = $(wildcard $(PATH_TEST_FILES)*.c)
# List of all .c files to be compiled
SRC_FILES = $(wildcard $(PATH_SRC)*.c) $(wildcard $(PATH_TEST_FILES)*.c) $(wildcard $(PATH_UNITY)*.c)
# List of all object files I'm expecting
OBJ_FILES = $(patsubst %.c,$(PATH_OBJECT_FILES)%.o, $(notdir $(SRC_FILES)))
# List of all the result output .txt files from the build
RESULTS = $(patsubst $(PATH_TEST_FILES)test_%.c, $(PATH_RESULTS)test_%.txt, $(SRC_TEST_FILES))

# Other constants
MAIN_TARGET_NAME = lin_pid

# Compiler setup
CROSS	= 
CC = $(CROSS)gcc
COMPILER_WARNING_FLAGS = -Wall -Wextra -Wconversion -Wdouble-promotion -Wnull-dereference -Wwrite-strings -Wformat=2 -Wcast-align=strict -Wswitch-enum -Wpedantic
# COMPILER_SANITIZERS =
COMPILER_SANITIZERS = -fsanitize=bool -fsanitize=undefined -fsanitize-trap
DEFAULT_COMPILER_OPTIMIZATION_LEVEL = -Og -g3
COMPILER_OPTIMIZATION_LEVEL_SPEED = -O3
COMPILER_OPTIMIZATION_LEVEL_SPACE = -Os
COMPILER_STANDARD = -std=c99
INCLUDE_PATHS = -I. -I$(PATH_SRC) -I$(PATH_UNITY)
DEFINES = -DTEST
# TODO: Add separate defines for release build
CFLAGS = $(INCLUDE_PATHS) $(DEFINES) $(COMPILER_WARNING_FLAGS) $(COMPILER_SANITIZERS) $(DEFAULT_COMPILER_OPTIMIZATION_LEVEL) $(COMPILER_STANDARD)
CFLAGS_FAST = $(INCLUDE_PATHS) $(COMPILER_WARNING_FLAGS) $(COMPILER_OPTIMIZATION_LEVEL_SPEED) $(COMPILER_STANDARD)
CFLAGS_SMALL = $(COMPILER_WARNING_FLAGS) $(COMPILER_OPTIMIZATION_LEVEL_SPACE) $(COMPILER_STANDARD)
LDFLAGS = 

## The Rules & Recipes

.PHONY: target
target: $(PATH_BUILD)$(MAIN_TARGET_NAME).$(TARGET_EXTENSION)

.PHONY: test
test: $(BUILD_PATHS) $(RESULTS)

$(PATH_BUILD)$(MAIN_TARGET_NAME).exe: $(PATH_BUILD)$(MAIN_TARGET_NAME).o
	@echo
	@echo "----------------------------------------"
	@echo "Linking the object files $^ into the test executable..."
	@echo
	$(CC) $(LDFLAGS) $^ -o $@

$(PATH_BUILD)$(MAIN_TARGET_NAME).o: $(PATH_SRC)$(MAIN_TARGET_NAME).c $(PATH_SRC)$(MAIN_TARGET_NAME).h
	@echo
	@echo "----------------------------------------"
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS_FAST) $< -o $@
	@echo

# Write the test results to a result .txt file
$(PATH_RESULTS)%.txt: $(PATH_BUILD)%.$(TARGET_EXTENSION) $(PATH_BUILD)$(MAIN_TARGET_NAME).lst	# Don't actually need the .lst file but want to force the disassembly generation
	@echo
	@echo "----------------------------------------"
	@echo "Running $<..."
	@echo
	-./$< 2>&1 | tee $@ | python colorize_unity_output.py

# Produces an object dump that includes the disassembly of the executable
$(PATH_BUILD)%.lst: $(PATH_OBJECT_FILES)%.o
	@echo
	@echo "----------------------------------------"
	@echo "Disassembly of $<..."
	@echo
	objdump -D $< > $@

# Build the executable
$(PATH_BUILD)%.$(TARGET_EXTENSION): $(OBJ_FILES)
	@echo
	@echo "----------------------------------------"
	@echo "Linking the object files $^ into the test executable..."
	@echo
	$(CC) $(LDFLAGS) $^ -o $@

$(PATH_OBJECT_FILES)%.o: $(PATH_SRC)%.c $(PATH_SRC)%.h
	@echo
	@echo "----------------------------------------"
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS) $< -o $@
	@echo

$(PATH_OBJECT_FILES)%.o: $(PATH_TEST_FILES)%.c
	@echo
	@echo "----------------------------------------"
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS) $< -o $@
	@echo

$(PATH_OBJECT_FILES)%.o: $(PATH_UNITY)%.c
	@echo
	@echo "----------------------------------------"
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS) $< -o $@
	@echo

# Make the directories if they don't already exist
$(PATH_RESULTS):
	$(MKDIR) $@

$(PATH_OBJECT_FILES):
	$(MKDIR) $@

$(PATH_BUILD):
	$(MKDIR) $@

# Clean rule to remove generated files
.PHONY: clean
clean:
	$(CLEANUP) $(PATH_OBJECT_FILES)*.o
	$(CLEANUP) $(PATH_BUILD)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATH_RESULTS)*.txt
	$(CLEANUP) $(PATH_BUILD)*.lst

.PRECIOUS: $(PATH_BUILD)%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATH_BUILD)Test%.o
.PRECIOUS: $(PATH_RESULTS)%.txt
.PRECIOUS: $(PATH_RESULTS)%.lst
