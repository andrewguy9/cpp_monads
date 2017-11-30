CXX = clang++
CXX_FLAGS = -Wfatal-errors -Wall -Wextra -Wpedantic -Wconversion -Wshadow -std=c++11

# Final binary
BIN = calc_tests.out maybe_tests.out
# Put all auto generated stuff to this build dir.
BUILD_DIR = ./build

# List of all .cpp source files.
CPP = calc_tests.cpp maybe_tests.cpp $(wildcard dir1/*.cpp) $(wildcard dir2/*.cpp)

# All .o files go to build dir.
OBJ = $(CPP:%.cpp=$(BUILD_DIR)/%.o)
# Gcc/Clang will create these .d files containing dependencies.
DEP = $(OBJ:%.o=%.d)

# Default target named after the binary.
# $(BIN) : $(BUILD_DIR)/$(BIN)

# # Actual target of the binary - depends on all .o files.
# $(BUILD_DIR)/$(BIN) : $(OBJ)
# 	# Create build directories - same structure as sources.
# 	mkdir -p $(@D)
# 	# Just link all the object files.
# 	$(CXX) $(CXX_FLAGS) $^ -o $@

.PHONY : clean
all: $(BUILD_DIR)/calc_tests.out $(BUILD_DIR)/maybe_tests.out

$(BUILD_DIR)/calc_tests.out : $(BUILD_DIR)/calc_tests.o
	# Create build directories - same structure as sources.
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $^ -o $@

$(BUILD_DIR)/maybe_tests.out : $(BUILD_DIR)/maybe_tests.o
	# Create build directories - same structure as sources.
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $^ -o $@

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
	-rm $(BUILD_DIR)/$(BIN) $(OBJ) $(DEP)
