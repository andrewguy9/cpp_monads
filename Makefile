CXX = clang++
CXX_FLAGS = -Wfatal-errors -Wall -Wextra -Wpedantic -Wconversion -Wshadow -std=c++11 -g

HSCC = ghc
HSCC_FLAGS = -hidir $(BUILD_DIR) -odir $(BUILD_DIR)

# List of final binaries
BINS = calc.out calc_tests.out maybe_tests.out hcalc.out
# Put all auto generated stuff to this build dir.
BUILD_DIR = ./build

# List of all .cpp source files.
CPP = calc_tests.cpp maybe_tests.cpp calc.cpp $(wildcard dir1/*.cpp) $(wildcard dir2/*.cpp)

# List of all .hs source files.
HS = hcalc.hs

# All .out files go to build dir.
OUT = $(BINS:%.out=$(BUILD_DIR)/%.out)
# All .o files go to build dir.
OBJ = $(CPP:%.cpp=$(BUILD_DIR)/%.o)
# All .hi files go to the build dir.
HI = $(HS:%.hs=$(BUILD_DIR)/%.hi)
# Gcc/Clang will create these .d files containing dependencies.
DEP = $(OBJ:%.o=%.d)

.PHONY : clean
all: $(BINS)

calc_tests.out : $(BUILD_DIR)/calc_tests.o
	# Create build directories - same structure as sources.
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $^ -o $@

maybe_tests.out : $(BUILD_DIR)/maybe_tests.o
	# Create build directories - same structure as sources.
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $^ -o $@

calc.out : $(BUILD_DIR)/calc.o
	# Create build directories - same structure as sources.
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $^ -o $@

hcalc.out : hcalc.hs
	# Create build directories - same structure as sources.
	mkdir -p $(@D)
	$(HSCC) $(HSCC_FLAGS) $^ -o $@

# Include all .d files
-include $(DEP)

# Build target for every single object file.
# The potential dependency on header files is covered
# by calling `-include $(DEP)`.
$(BUILD_DIR)/%.o : %.cpp
	mkdir -p $(@D)
	# The -MMD flags additionaly creates a .d file with
	# the same name as the .o file.
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

.PHONY : clean
clean :
	# This should remove all generated files.
	-rm $(BINS) $(OBJ) $(DEP) $(HI)
