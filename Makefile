CXX := g++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra -Wpedantic
LDFLAGS :=

SRC_DIR := src
BIN_DIR := bin
TARGET := $(BIN_DIR)/p2

SRCS := $(SRC_DIR)/p2.cpp $(SRC_DIR)/image.cpp
OBJS := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/image.h $(SRC_DIR)/DisjSets.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

.PHONY: all clean