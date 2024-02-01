CCOMPILE = g++

# Functions:
# Generate file path of object for a given source file
targ_from_cpp = $(addprefix build/,$(1:.cpp=.o))
# Generate file path of dependency for a given object file
deps_from_obj = $(1:.o=.d)

# File Lists:
# All c++ sources to compile
CPP_SRCS = $(shell find -name '*.cpp' -printf '%P ')
# All objects to compile
OBJS = $(call targ_from_cpp,$(CPP_SRCS))
# All dependencies to read
DEPS = $(call deps_from_obj,$(OBJS))
# Common cpp files 
COMMON_OBJS = $(filter-out build/common/testing/%,$(filter build/common/%.o,$(OBJS)))

# Reads dependencies of each object that has already been compiled
# - Allows modifications to headers to be detected so objects can be recompiled
-include $(DEPS)

# Compile c++ sources to object files
# - Make directory to store output
# - Compile object
# - Register dependencies (eg headers) to a '.d' file
build/%.o : %.cpp
	mkdir -p $(dir $@)
	$(CCOMPILE) -c $< -o $@ -MMD -MP -MT $@

build/common_%.exe : build/common/testing/%.o $(COMMON_OBJS)
	$(CCOMPILE) -o $@ $^


.PHONY = clean, wipe, objects, tests, common_bench, cacheFactorials, test2


test2:
	echo $(OBJS)
	echo $(DEPS)
	echo hello

# Remove all objects and dependency files
clean:
	find ./build/ -name '*.[od]' -delete

# Completely empty /build/ except for the .gitignore
wipe:
	find ./build/ ! -name 'build' ! -name '*.gitignore' -delete

# Compile all object files
objects: $(OBJS)

# Run all tests
tests: build/common_unittests.exe
	for test_exe in $^ ; do ./$$test_exe; done

cacheFactorials: build/cacheFactorials.exe
	$<

build/cacheFactorials.exe: build/GPUExperimentation/cacheFactorials.o
	$(CCOMPILE) -o $@ $^


common_bench: build/common_benchmarks.exe
	$<