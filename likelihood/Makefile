SOURCE_ROOT = src

ENTRY_DIR = entrypoints
TEST_DIR = tests
BENCH_DIR = benchmarks

BUILD_ROOT = build

OBJ_ROOT = $(BUILD_ROOT)/partial
EXE_ROOT = $(BUILD_ROOT)/run

CCOMPILE = g++ -I$(SOURCE_ROOT) -I../Catch2

ENTRY_OBJ_ROOT = $(OBJ_ROOT)/$(ENTRY_DIR)

TEST_EXE_ROOT = $(EXE_ROOT)/$(TEST_DIR)
BENCH_EXE_ROOT = $(EXE_ROOT)/$(BENCH_DIR)

# Functions:
# Generate file path of object for a given source file
obj_from_cpp = $(1:$(SOURCE_ROOT)%.cpp=$(OBJ_ROOT)%.o)

# Generate file path of dependency for a given object file
deps_from_obj = $(1:.o=.d)

exe_from_obj = $(1:$(OBJ_ROOT)/$(ENTRY_DIR)/%.o=$(EXE_ROOT)/%.exe)

# File Lists:
# All c++ sources to compile
CPP_SRCS = $(shell find -name '*.cpp' -printf '%P ')
# All objects to compile
OBJS = $(call obj_from_cpp,$(CPP_SRCS))
# All dependencies to read
DEPS = $(call deps_from_obj,$(OBJS))

COMPONENT_OBJS = $(filter-out $(ENTRY_OBJ_ROOT)/%,$(OBJS))

EXES = $(call exe_from_obj,$(filter $(ENTRY_OBJ_ROOT)/%,$(OBJS)))

TEST_EXES = $(filter $(TEST_EXE_ROOT)/%,$(EXES))
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
	$(CCOMPILE) -c $< -o $@ -MMD -MP -MT $@

$(EXE_ROOT)/%.exe : $(ENTRY_OBJ_ROOT)/%.o $(COMPONENT_OBJS)
	mkdir -p $(dir $@)
	$(CCOMPILE) -o $@ $^

.PHONY = clean, wipe, objects, tests, benches

# Remove all objects and dependency files
clean:
	find $(BUILD_ROOT) -name '*.[od]' -delete

# Completely empty /build/ except for the .gitignore
wipe:
	find $(BUILD_ROOT) ! -name 'build' ! -name '*.gitignore' -delete

# Compile all object files
objects: $(OBJS)

# Run all tests
tests: $(TEST_EXES)
	for test_exe in $^ ; do ./$$test_exe; done

# Run all benchmarks
benches: $(BENCH_EXES)
	for bench_exe in $^ ; do ./$$bench_exe; done