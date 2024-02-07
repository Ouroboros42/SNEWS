# Location of whole project
ROOT := .

# Directory of cpp source files
SOURCE_DIR := src

# Directory in source root for files with main
ENTRY_DIR := $(SOURCE_DIR)/entrypoints

# Directories in entry dir for tests and benchmarks 
TEST_DIR := tests
BENCH_DIR := benchmarks

# Directory of external libaries
LIBS_DIR := libs

# Directory to put build products
BUILD_DIR := build

# Directories in build dir for objects and executables
OBJ_DIR := partial
EXE_DIR := run

# Generated paths:
SOURCE_ROOT := $(ROOT)/$(SOURCE_DIR)
BUILD_ROOT := $(ROOT)/$(BUILD_DIR)
LIBS_ROOT := $(ROOT)/$(LIBS_DIR)
OBJ_ROOT := $(BUILD_ROOT)/$(OBJ_DIR)
EXE_ROOT := $(BUILD_ROOT)/$(EXE_DIR)
ENTRY_OBJ_ROOT := $(OBJ_ROOT)/$(ENTRY_DIR)
TEST_EXE_ROOT := $(EXE_ROOT)/$(TEST_DIR)
BENCH_EXE_ROOT := $(EXE_ROOT)/$(BENCH_DIR)

# Paths to search for gcc includes
C_INCLUDES := $(SOURCE_ROOT) $(wildcard $(LIBS_ROOT)/*)

# Command to compile c++
C_COMPILE := g++ $(addprefix -I,$(C_INCLUDES))

# Arguments passed to test exes
TEST_ARGS := -d yes

# Functions:
# Generate file path of object for a given source file
obj_from_cpp = $(1:$(ROOT)/%.cpp=$(OBJ_ROOT)/%.o)

# Generate file path of dependency list for a given object file
deps_from_obj = $(1:%.o=%.d)

# Generate file path of target executable for given entrypoint object file
exe_from_obj = $(1:$(OBJ_ROOT)/$(ENTRY_DIR)/%.o=$(EXE_ROOT)/%.exe)

# Generate file path of object defining variables of header (assumes source files have same name with differing extensions)
obj_from_header = $(call obj_from_cpp,$(1:%.hpp=%.cpp))

# Run all executables listed in argument
run_all = for exe in $1 ; do $$exe $2; done

run_tests = $(call run_all,$1,$(TEST_ARGS))

# File Lists:
# All c++ sources to compile
CPP_SRCS := $(shell find $(ROOT) -name '*.cpp')
# All objects to compile
OBJS := $(call obj_from_cpp,$(CPP_SRCS))
# All dependencies to read
DEPS := $(call deps_from_obj,$(OBJS))
# Reuseable objects to compile into all executables
COMPONENT_OBJS := $(filter-out $(ENTRY_OBJ_ROOT)/%,$(OBJS))
# Objects to be turn into main of executables
EXE_OBJS := $(filter $(ENTRY_OBJ_ROOT)/%,$(OBJS))
# All executables to compile
EXES := $(call exe_from_obj,$(EXE_OBJS))
# Executables with unit tests
TEST_EXES := $(filter $(TEST_EXE_ROOT)/%,$(EXES))
# Executables with benchmarks
BENCH_EXES := $(filter $(BENCH_EXE_ROOT)/%,$(EXES))

# Reads dependencies of each object that has already been compiled
# - Allows modifications to headers to be detected so objects can be recompiled
-include $(DEPS)

# Compile c++ sources to object files
# - Make directory to store output
# - Compile object
# - Register dependencies (eg headers) to a '.d' file
$(OBJ_ROOT)/%.o : $(ROOT)/%.cpp
	mkdir -p $(dir $@)
	$(C_COMPILE) -c $< -o $@ -MMD -MP -MT $@

## Shennanigans to deduce which objects have which headers
include_dot_root = $(addprefix $(ROOT)/,$(1:$(ROOT)/%=%))
export OBJS DEPS
get_direct_header_objs = $(filter $(OBJS),$(call obj_from_header,$(call include_dot_root,$(shell $(MAKE) -s -f get_headers.mk $1))))
extend_while_nonempty_internal = $(if $3,$(call extend_while_nonempty,$1,$2 $3),$2 $3)
extend_while_nonempty = $(call extend_while_nonempty_internal,$1,$2,$(filter-out $2,$(call $1,$2)))
get_header_objs = $(call extend_while_nonempty,get_direct_header_objs,$1)

# Compile c++ objects to executable
# - Make directory to store output
# - Compile one file from entrypoints with all reuseable objects to a '.exe'
# Dynamically defines dependent objects using '.d' files, so must be run after making all objects
define EXE_template =
$(call exe_from_obj,$1) : $(call get_header_objs,$1)
	mkdir -p $$(dir $$@)
	$$(C_COMPILE) -o $$@ $$^
endef
$(foreach obj,$(EXE_OBJS),$(eval $(call EXE_template,$(obj))))

# Tracks last time tests were run for mods command
build/last_test: $(TEST_EXES)
	touch $@
	$(call run_tests,$?)

# Shorthand commands
.PHONY : clean, wipe, objects, executables, tests, benches, mods, end2end, end2endExp

# Completely empty /build/ except for the .gitignore
wipe:
	find $(BUILD_ROOT) ! -wholename '$(BUILD_ROOT)' ! -name '*.gitignore' -delete

# Compile all object files
objects: $(OBJS)

# Compile all executables
executables: $(EXES)

# Run all tests
tests: $(TEST_EXES)
	$(call run_tests,$^)

# Run all benchmarks
benches: $(BENCH_EXES)
	$(call run_tests,$^)

# Run all modified tests and benchmarks
mods: build/last_test

end2end : build/run/end2end.exe
	$<

end2endExp : build/run/end2endExp.exe
	$<