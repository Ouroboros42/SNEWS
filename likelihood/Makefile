# Location of cpp source files
SOURCE_ROOT = src

# Directory in source root for files with main
ENTRY_DIR = entrypoints

# Directories in entry dir for tests and benchmarks 
TEST_DIR = tests
BENCH_DIR = benchmarks

# Location to put build products
BUILD_ROOT = build

# Directories in build dir for objects and executables
OBJ_DIR = partial
EXE_DIR = run

# Paths to search for gcc includes
C_INCLUDES = $(SOURCE_ROOT) ../Catch2

# Command to compile c++
C_COMPILE = g++ $(addprefix -I,$(C_INCLUDES))

# Generated paths:
OBJ_ROOT = $(BUILD_ROOT)/$(OBJ_DIR)
EXE_ROOT = $(BUILD_ROOT)/$(EXE_DIR)
ENTRY_OBJ_ROOT = $(OBJ_ROOT)/$(ENTRY_DIR)
TEST_EXE_ROOT = $(EXE_ROOT)/$(TEST_DIR)
BENCH_EXE_ROOT = $(EXE_ROOT)/$(BENCH_DIR)

# Functions:
# Generate file path of object for a given source file
obj_from_cpp = $(1:$(SOURCE_ROOT)%.cpp=$(OBJ_ROOT)%.o)

# Generate file path of dependency list for a given object file
deps_from_obj = $(1:.o=.d)

# Generate file path of target executable for given entrypoint object file
exe_from_obj = $(1:$(OBJ_ROOT)/$(ENTRY_DIR)/%.o=$(EXE_ROOT)/%.exe)

# Run all executables listed in argument
run_all = for exe in $1 ; do ./$$exe; done

# File Lists:
# All c++ sources to compile
CPP_SRCS = $(shell find -name '*.cpp' -printf '%P ')
# All objects to compile
OBJS = $(call obj_from_cpp,$(CPP_SRCS))
# All dependencies to read
DEPS = $(call deps_from_obj,$(OBJS))
# Reuseable objects to compile into all executables
COMPONENT_OBJS = $(filter-out $(ENTRY_OBJ_ROOT)/%,$(OBJS))
# All executables to compile
EXES = $(call exe_from_obj,$(filter $(ENTRY_OBJ_ROOT)/%,$(OBJS)))
# Executables with unit tests
TEST_EXES = $(filter $(TEST_EXE_ROOT)/%,$(EXES))
# Executables with benchmarks
BENCH_EXES = $(filter $(BENCH_EXE_ROOT)/%,$(EXES))

# Reads dependencies of each object that has already been compiled
# - Allows modifications to headers to be detected so objects can be recompiled
-include $(DEPS)

# Compile c++ sources to object files
# - Make directory to store output
# - Compile object
# - Register dependencies (eg headers) to a '.d' file
$(OBJ_ROOT)/%.o : $(SOURCE_ROOT)/%.cpp
	mkdir -p $(dir $@)
	$(C_COMPILE) -c $< -o $@ -MMD -MP -MT $@

# Compile c++ objects to executable
# - Make directory to store output
# - Compile one file from entrypoints with all reuseable objects to a '.exe'
$(EXE_ROOT)/%.exe : $(ENTRY_OBJ_ROOT)/%.o $(COMPONENT_OBJS)
	mkdir -p $(dir $@)
	$(C_COMPILE) -o $@ $^

# Tracks last time tests were run for mods command
build/last_test: $(TEST_EXES) $(BENCH_EXES)
	touch $@
	$(call run_all,$?)

# Shorthand commands
.PHONY = clean, wipe, objects, executables, tests, benches, mods

# Remove all objects and dependency files
clean:
	find $(BUILD_ROOT) -name '*.[od]' -delete

# Completely empty /build/ except for the .gitignore
wipe:
	find $(BUILD_ROOT) ! -wholename '$(BUILD_ROOT)' ! -name '*.gitignore' -delete

# Compile all object files
objects: $(OBJS)

# Compile all executables
executables: $(EXES)

# Run all tests
tests: $(TEST_EXES)
	$(call run_all,$^)

# Run all benchmarks
benches: $(BENCH_EXES)
	$(call run_all,$^)

# Run all modified tests and benchmarks
mods: build/last_test