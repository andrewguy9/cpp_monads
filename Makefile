CXX = clang++
CXX_FLAGS = -Wfatal-errors -Wall -Wextra -Wpedantic -Wconversion -Wshadow -std=c++11 -g

HSCC = ghc
HSCC_FLAGS = -hidir $(BUILD_DIR) -odir $(BUILD_DIR)

# List of final binaries
BINS = bool_tests.out calc.out calc_tests.out maybe_tests.out functional_tests.out hcalc.out
# Put all auto generated stuff to this build dir.
BUILD_DIR = ./build

# List of all .cpp source files.
CPP = bool_tests.cpp calc_tests.cpp maybe_tests.cpp functional_tests.cpp calc.cpp $(wildcard dir1/*.cpp) $(wildcard dir2/*.cpp)

# List of all .hs source files.
HS = hcalc.hs

# All .out files go to build dir.
OUT = $(BINS:%.out=$(BUILD_DIR)/%.out)
# All .o files go to build dir.
OBJ = $(CPP:%.cpp=$(BUILD_DIR)/%.o) $(BUILD_DIR)/Main.o
# All .hi files go to the build dir.
HI = $(BUILD_DIR)/Main.hi
# Gcc/Clang will create these .d files containing dependencies.
DEP = $(CPP:%.cpp=$(BUILD_DIR)/%.d)

.PRECIOUS: $(BUILD_DIR)/%.o

.PHONY : all clean tests
all: $(BINS)

hcalc.out   : hcalc.hs
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

# Build executible target for every single object file.
%.out : $(BUILD_DIR)/%.o
	$(CXX) $(CXX_FLAGS) $^ -o $@

clean:
	# This should remove all generated files.
	-rm $(BINS) $(OBJ) $(DEP) $(HI)

tests: bool_tests.out calc_tests.out functional_tests.out maybe_tests.out
	./bool_tests.out
	./calc_tests.out
	./functional_tests.out
	./maybe_tests.out

