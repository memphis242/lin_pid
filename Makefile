.POSIX:

.PHONY: test
.PHONY: release debug benchmark profile
.PHONY: target
.PHONY: clean
.PHONY: clean_target

test:
	@echo "Building code if applicable... Otherwise, straight to tests."
	@$(MAKE) BUILD_TYPE=TEST _test > /dev/null
	cat $(RESULTS) | python $(COLORIZE_UNITY_SCRIPT)

release:
	@$(MAKE) target BUILD_TYPE=RELEASE

debug:
	@$(MAKE) target BUILD_TYPE=DEBUG

benchmark:
	@$(MAKE) _benchmark BUILD_TYPE=BENCHMARK

profile:
	@$(MAKE) _test BUILD_TYPE=PROFILE

################################# The Prelude ##################################

CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION=exe
# Set the OS-specific tool cmds / executable extensions
ifeq ($(OS),Windows_NT)

  TARGET_EXTENSION = exe
  STATIC_LIB_EXTENSION = lib

  ifeq ($(shell uname -s),) # not in a bash-like shell
    CLEANUP = del /F /Q
    MKDIR = mkdir
  else # in a bash-like shell, like msys
    CLEANUP = rm -f
    MKDIR = mkdir -p
  endif

else

  TARGET_EXTENSION = out
  STATIC_LIB_EXTENSION = a
  CLEANUP = rm -f
  MKDIR = mkdir -p

endif

# Relevant paths
PATH_UNITY        = Unity/src/
PATH_SRC          = src/
PATH_TINY_REGEX	= $(PATH_SRC)tiny-regex-c/
PATH_INC          = $(PATH_SRC)/
PATH_TEST_FILES   = test/
PATH_BUILD        = build/
PATH_OBJECT_FILES = $(PATH_BUILD)objs/
PATH_RESULTS      = $(PATH_BUILD)results/
PATH_PROFILE      = $(PATH_BUILD)profile/
PATH_BENCHMARK	   = benchmark/
PATH_SCRIPTS      = scripts/
BUILD_DIRS        = $(PATH_BUILD) $(PATH_OBJECT_FILES)

# List of all the build paths
# The pattern employed here is to generate lists of files which shall then be
# used as pre-requisities in downstream rules.
COLORIZE_CPPCHECK_SCRIPT = $(PATH_SCRIPTS)colorize_cppcheck.py
COLORIZE_UNITY_SCRIPT = $(PATH_SCRIPTS)colorize_unity.py

# Other constants
MAIN_TARGET_NAME = lin_pid

# List of all the test .c files
SRC_TEST_FILES = $(wildcard $(PATH_TEST_FILES)*.c)
# List of all the result output .txt files from the build
RESULTS = $(patsubst $(PATH_TEST_FILES)%.c, $(PATH_RESULTS)%.txt, $(SRC_TEST_FILES))

ifeq ($(BUILD_TYPE), TEST)

BUILD_PATHS = $(PATH_BUILD) $(PATH_OBJECT_FILES) $(PATH_RESULTS)
# List of all .c files to be compiled
MAIN_SRC_FILES = $(wildcard $(PATH_SRC)*.c)
SRC_FILES = $(MAIN_SRC_FILES) \
            $(wildcard $(PATH_TEST_FILES)*.c) \
            $(wildcard $(PATH_UNITY)*.c) \
            $(wildcard $(PATH_TINY_REGEX)*.c)
# List of all gcov coverage files I'm expecting
GCOV_FILES = $(MAIN_SRC_FILES:.c=.c.gcov)

else

BUILD_PATHS = $(PATH_BUILD) $(PATH_OBJECT_FILES)
# List of all .c files to be compiled
SRC_FILES = $(wildcard $(PATH_SRC)*.c) \
				$(wildcard $(PATH_TINY_REGEX)*.c)

endif

ifeq ($(BUILD_TYPE), PROFILE)
BUILD_PATHS += $(PATH_PROFILE)
endif

# List of all object files we're expecting for the data structures
OBJ_FILES = $(patsubst %.c,$(PATH_OBJECT_FILES)%.o, $(notdir $(SRC_FILES)))

# Compiler setup
CROSS	= 
CC = $(CROSS)gcc

COMPILER_WARNING_FLAGS = \
    -Wall -Wextra -Wpedantic -pedantic-errors \
    -Wconversion -Wdouble-promotion -Wnull-dereference \
    -Wwrite-strings -Wformat=2 -Wformat-overflow=2 \
    -Wformat-signedness -Wuseless-cast -Wstrict-prototypes \
    -Wcast-align=strict -Wimplicit-fallthrough=3 -Wswitch-default \
    -Wswitch-enum -Wfloat-equal -Wuse-after-free=2 \
    -Wdeprecated-declarations -Wmissing-prototypes -Wparentheses \
    -Wreturn-type -Wlogical-op -Wstrict-aliasing \
    -Wuninitialized -Wmaybe-uninitialized -Wshadow \
    -Wduplicated-cond -Wduplicated-branches \
    -Walloc-zero -Walloc-size -Wpacked

# Includes some -Wno-... flags for warnings that I'd normally want for my lib
# src but **not** for my test file, which intentionally has all sorts of
# naughty shenanigans going on
COMPILER_WARNINGS_TEST_BUILD_TEST_FILES = \
    -Wall -Wextra -Wpedantic -pedantic-errors \
    -Wconversion -Wdouble-promotion -Wnull-dereference \
    -Wwrite-strings -Wformat=2 -Wformat-overflow=2 \
    -Wformat-signedness \
    -Wcast-align=strict -Wimplicit-fallthrough=3 -Wswitch-default \
    -Wswitch-enum -Wfloat-equal -Wuse-after-free=2 \
    -Wdeprecated-declarations -Wmissing-prototypes -Wparentheses \
    -Wreturn-type -Wlogical-op -Wstrict-aliasing \
    -Wuninitialized -Wmaybe-uninitialized -Wshadow \
    -Wsuggest-attribute=const \
    -Walloc-zero -Walloc-size \
    -Wno-analyzer-use-of-uninitialized-value -Wno-uninitialized \
    -Wno-maybe-uninitialized -Wno-useless-cast

COMPILER_WARNINGS_TEST_BUILD_SRC_FILES = \
    -Wall -Wextra -Wpedantic -pedantic-errors \
    -Wconversion -Wdouble-promotion -Wnull-dereference \
    -Wwrite-strings -Wformat=2 -Wformat-overflow=2 \
    -Wformat-signedness \
    -Wcast-align=strict -Wimplicit-fallthrough=3 -Wswitch-default \
    -Wswitch-enum -Wfloat-equal -Wuse-after-free=2 \
    -Wdeprecated-declarations -Wmissing-prototypes -Wparentheses \
    -Wreturn-type -Wlogical-op -Wstrict-aliasing \
    -Wuninitialized -Wmaybe-uninitialized -Wshadow \
    -Wsuggest-attribute=const \
    -Walloc-zero -Walloc-size \
    -Wno-analyzer-use-of-uninitialized-value -Wno-uninitialized \
    -Wno-maybe-uninitialized -Wno-useless-cast \
	 -fcondition-coverage -fprofile-arcs -ftest-coverage

# Consider -Wmismatched-dealloc
COMPILER_SANITIZERS = \
    -fsanitize=undefined -fsanitize-trap \
    -fsanitize=enum  -fsanitize=bool -fsanitize=bounds
COMPILER_OPTIMIZATION_LEVEL_DEBUG = -Og -g3
COMPILER_OPTIMIZATION_LEVEL_SPEED = -O3
COMPILER_OPTIMIZATION_LEVEL_SPACE = -Os
COMPILER_STANDARD = -std=c99
INCLUDE_PATHS = -I. -I$(PATH_INC) -I$(PATH_UNITY) -I$(PATH_TINY_REGEX)
COMMON_DEFINES =
DIAGNOSTIC_FLAGS = -fdiagnostics-color
COMPILER_STATIC_ANALYZER = -fanalyzer

# Compile up the compiler flags
CFLAGS_SRC_FILES  = $(INCLUDE_PATHS) $(COMMON_DEFINES) $(DIAGNOSTIC_FLAGS) $(COMPILER_STANDARD)
CFLAGS_TEST_FILES = $(INCLUDE_PATHS) $(COMMON_DEFINES) $(DIAGNOSTIC_FLAGS) $(COMPILER_STANDARD)

ifeq ($(BUILD_TYPE), RELEASE)
CFLAGS_SRC_FILES  += -DNDEBUG $(COMPILER_WARNING_FLAGS) $(COMPILER_STATIC_ANALYZER) $(COMPILER_OPTIMIZATION_LEVEL_SPEED)
CFLAGS_TEST_FILES += -DNDEBUG $(COMPILER_WARNING_FLAGS) $(COMPILER_STATIC_ANALYZER) $(COMPILER_OPTIMIZATION_LEVEL_SPEED)

else ifeq ($(BUILD_TYPE), TEST)
CFLAGS_SRC_FILES  += -DTEST $(COMPILER_SANITIZERS) $(COMPILER_WARNINGS_TEST_BUILD_SRC_FILES) $(COMPILER_STATIC_ANALYZER) $(COMPILER_OPTIMIZATION_LEVEL_DEBUG)
CFLAGS_TEST_FILES += -DTEST $(COMPILER_SANITIZERS) $(COMPILER_WARNINGS_TEST_BUILD_TEST_FILES) $(COMPILER_STATIC_ANALYZER) $(COMPILER_OPTIMIZATION_LEVEL_DEBUG)

else ifeq ($(BUILD_TYPE), BENCHMARK)
CFLAGS += -DNDEBUG $(COMPILER_WARNING_FLAGS) $(COMPILER_STATIC_ANALYZER) $(COMPILER_OPTIMIZATION_LEVEL_SPEED)

else ifeq ($(BUILD_TYPE), PROFILE)
CFLAGS += -DNDEBUG $(COMPILER_WARNING_FLAGS) $(COMPILER_STATIC_ANALYZER) $(COMPILER_OPTIMIZATION_LEVEL_DEBUG) -pg
LDFLAGS += -pg

else
CFLAGS_SRC_FILES  += $(COMPILER_SANITIZERS) $(COMPILER_WARNING_FLAGS) $(COMPILER_STATIC_ANALYZER) $(COMPILER_OPTIMIZATION_LEVEL_DEBUG)
CFLAGS_TEST_FILES += $(COMPILER_SANITIZERS) $(COMPILER_WARNING_FLAGS) $(COMPILER_STATIC_ANALYZER) $(COMPILER_OPTIMIZATION_LEVEL_DEBUG)
endif

# Compile up linker flags
LDFLAGS += $(DIAGNOSTIC_FLAGS)
ifeq ($(BUILD_TYPE), TEST)
LDFLAGS += -lgcov --coverage
endif

# CppCheck Flags
#CPPCHECK_FLAGS = --check-level=exhaustive --cppcheck-build-dir=$(PATH_BUILD)
CPPCHECK_FLAGS = --cppcheck-build-dir=$(PATH_BUILD)

# gcov Flags
GCOV = gcov
GCOV_FLAGS = --conditions --function-summaries --branch-probabilities --branch-counts
ifeq ($(GCOV_CON), 1)
GCOV_FLAGS += --use-colors --stdout
endif
GCOV_CONSOLE_OUT_FILE = gcov_console_out.txt

# gcovr Flags
GCOVR_FLAGS = --html-details $(PATH_RESULTS)coverage.html

############################# The Rules & Recipes ##############################

target: $(BUILD_PATHS) $(PATH_BUILD)$(MAIN_TARGET_NAME).$(TARGET_EXTENSION)
	@echo
	@echo -e "\033[36mTarget successfully built!\033[0m"
	@echo

_test: $(BUILD_PATHS) $(RESULTS) $(GCOV_FILES)
	@echo
	@echo -e "\033[36mAll tests completed!\033[0m"
	@echo

# Write the test results to a result .txt file
$(PATH_RESULTS)%.txt: $(PATH_BUILD)%.$(TARGET_EXTENSION) $(PATH_BUILD)$(MAIN_TARGET_NAME).lst	# Don't actually need the .lst file but want to force the disassembly generation
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mRunning\033[0m $<..."
	@echo
	-./$< 2>&1 | tee $@ | python $(COLORIZE_UNITY_SCRIPT)

# Produces an object dump that includes the disassembly of the executable
$(PATH_BUILD)%.lst: $(PATH_OBJECT_FILES)%.o
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mDisassembly\033[0m of $< into $@..."
	@echo
	objdump -D $< > $@

$(PATH_BUILD)$(MAIN_TARGET_NAME).$(TARGET_EXTENSION): $(OBJ_FILES)
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mLinking\033[0m the object files $^ into the executable..."
	@echo
	$(CC) $(LDFLAGS) $^ -o $@

$(PATH_BUILD)%.$(TARGET_EXTENSION): $(OBJ_FILES)
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mLinking\033[0m the object files $^ into the executable..."
	@echo
	$(CC) $(LDFLAGS) $^ -o $@

$(PATH_OBJECT_FILES)%.o: $(PATH_SRC)%.c $(PATH_SRC)%.h $(PATH_SRC)lin_pid_exceptions.h $(PATH_SRC)lin_pid_supported_formats.h
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mCompiling\033[0m the main program source files: $<..."
	@echo
	$(CC) -c $(CFLAGS_SRC_FILES) $< -o $@
	@echo
ifeq ($(STATIC_ANALYSIS),1) # FIXME: Need an actual solution here to enabling/disabling static analysis for the test run...
		@echo "----------------------------------------"
		@echo -e "\033[36mRunning static analysis\033[0m on $<..."
		@echo
		cppcheck $(CPPCHECK_FLAGS) --template='{severity}: {file}:{line}: {message}' $< 2>&1 | tee $(PATH_BUILD)cppcheck.log | python $(COLORIZE_CPPCHECK_SCRIPT)
		@echo
endif

$(PATH_OBJECT_FILES)%.o: $(PATH_TINY_REGEX)%.c $(PATH_TINY_REGEX)%.h
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mCompiling\033[0m the main program source files: $<..."
	@echo
	$(CC) -c $(CFLAGS_SRC_FILES) $< -o $@
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mRunning static analysis\033[0m on $<..."
	@echo
	cppcheck $(CPPCHECK_FLAGS) --template='{severity}: {file}:{line}: {message}' $< 2>&1 | tee $(PATH_BUILD)cppcheck.log | python $(COLORIZE_CPPCHECK_SCRIPT)
	@echo

$(PATH_OBJECT_FILES)%.o: $(PATH_TEST_FILES)%.c
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mCompiling\033[0m the test source files: $<..."
	@echo
	$(CC) -c $(CFLAGS_TEST_FILES) $< -o $@
	@echo

$(PATH_OBJECT_FILES)%.o: $(PATH_UNITY)%.c $(PATH_UNITY)%.h
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mCompiling\033[0m the unity source files: $<..."
	@echo
	$(CC) -c $(CFLAGS_TEST_FILES) $< -o $@
	@echo


# NOTE:
# gcov seems very picky about how the directory to look for .gcno and .gcda
# files is specified. The string for the directory must utilize forward slashes
# '/', not back slashes '\', and must not end with a forward slash. Otherwise,
# gcov exists with a cryptic
# 		<obj_dir>/.gcno:cannot open notes file
# kind of error. Hence, I use $(<path>:%/=%) /w PATH_OBJECT_FILES.
#
# Also, I've redirected gcov's output because I want to prioritize viewing the
# unit test results. Coverage results are meant to be inspected manually rather
# than fed back immediately to the developer.

$(PATH_SRC)%.c.gcov: $(PATH_SRC)%.c $(PATH_SRC)%.h
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mAnalyzing coverage\033[0m for $<..."
	$(GCOV) $(GCOV_FLAGS) --object-directory $(PATH_OBJECT_FILES:%/=%) $< > $(PATH_RESULTS)$(GCOV_CONSOLE_OUT_FILE)
	mv *.gcov $(PATH_RESULTS)
	gcovr $(GCOVR_FLAGS)
	@echo

# Make the directories if they don't already exist
$(PATH_RESULTS):
	$(MKDIR) $@

$(PATH_OBJECT_FILES):
	$(MKDIR) $@

$(PATH_BUILD):
	$(MKDIR) $@

$(PATH_PROFILE):
	$(MKDIR) $@

# Clean rule to remove generated files
clean:
	@echo
	$(CLEANUP) $(PATH_OBJECT_FILES)*.o
	$(CLEANUP) $(PATH_OBJECT_FILES)*.gcda
	$(CLEANUP) $(PATH_OBJECT_FILES)*.gcno
	$(CLEANUP) $(PATH_BUILD)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATH_RESULTS)*.gcov
	$(CLEANUP) $(PATH_RESULTS)*.html
	$(CLEANUP) $(PATH_RESULTS)*.css
	$(CLEANUP) *.gcov
	$(CLEANUP) $(PATH_RESULTS)*.txt
	$(CLEANUP) $(PATH_BUILD)*.lst

clean_target:
	$(CLEANUP) $(PATH_OBJECT_FILES)$(MAIN_TARGET_NAME).o
	$(CLEANUP) $(PATH_BUILD)$(MAIN_TARGET_NAME).$(TARGET_EXTENSION)

.PRECIOUS: $(PATH_BUILD)%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATH_BUILD)Test%.o
.PRECIOUS: $(PATH_RESULTS)%.txt
.PRECIOUS: $(PATH_RESULTS)%.lst

_benchmark: $(PATH_BUILD)$(MAIN_TARGET_NAME).$(TARGET_EXTENSION) $(PATH_BUILD)lin_pid_sscanf.$(TARGET_EXTENSION)
	@echo
	@echo "----------------------------------------"
	@echo "Running $(MAIN_TARGET_NAME).$(TARGET_EXTENSION) 100 times and calculating the average, max, and min times..."
	@{ \
		total=0; \
		max=0; \
		min=999999; \
		for i in $$(seq 1 100); do \
			echo -ne "Run $$i...\r"; \
			time=$$( { time -f "%e" ./$(PATH_BUILD)$(MAIN_TARGET_NAME).$(TARGET_EXTENSION) 0x00 > /dev/null; } 2>&1 ); \
			time_ms=$$(echo "scale=6; $$time * 1000" | bc); \
			total=$$(echo "scale=6; $$total + $$time_ms" | bc); \
			if [ $$(echo "scale=6; $$time_ms > $$max" | bc) -eq 1 ]; then \
				max=$$time_ms; \
			fi; \
			if [ $$(echo "scale=6; $$time_ms < $$min" | bc) -eq 1 ]; then \
				min=$$time_ms; \
			fi; \
		done; \
		average=$$(echo "scale=6; $$total / 100" | bc); \
		echo "Average execution time: $$average ms"; \
		echo "Max execution time: $$max ms"; \
		echo "Min execution time: $$min ms"; \
		echo -e "----------------------------------------\n"; \
	}
	@echo "Running lin_pid_sscanf.$(TARGET_EXTENSION) 100 times and calculating the average, max, and min times..."
	@{ \
		total=0; \
		max=0; \
		min=999999; \
		for i in $$(seq 1 100); do \
			echo -ne "Run $$i...\r"; \
			time=$$( { time -f "%e" ./$(PATH_BUILD)lin_pid_sscanf.$(TARGET_EXTENSION) 0x00 > /dev/null; } 2>&1 ); \
			time_ms=$$(echo "scale=6; $$time * 1000" | bc); \
			total=$$(echo "scale=6; $$total + $$time_ms" | bc); \
			if [ $$(echo "scale=6; $$time_ms > $$max" | bc) -eq 1 ]; then \
				max=$$time_ms; \
			fi; \
			if [ $$(echo "scale=6; $$time_ms < $$min" | bc) -eq 1 ]; then \
				min=$$time_ms; \
			fi; \
		done; \
		average=$$(echo "scale=6; $$total / 100" | bc); \
		echo "Average execution time: $$average ms"; \
		echo "Max execution time: $$max ms"; \
		echo "Min execution time: $$min ms"; \
		echo -e "----------------------------------------\n"; \
	}
	@echo

