# constants
CXX = g++
CXXFLAGS = -Wall -g
CXXLIBS = -lSDL2

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

EXECUTABLE = c8oa

# targets
all: $(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)

# rules
$(BUILD_DIR)/$(EXECUTABLE): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(CXXLIBS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@