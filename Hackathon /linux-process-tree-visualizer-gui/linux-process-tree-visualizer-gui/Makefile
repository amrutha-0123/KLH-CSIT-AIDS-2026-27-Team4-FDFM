CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude
TARGET := process-collector
SOURCES := src/collector.cpp src/process_tree.cpp

.PHONY: all clean run gui
all: $(TARGET)

$(TARGET): $(SOURCES) include/process_tree.hpp
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: all
	python3 gui/app.py

gui: all
	python3 gui/app.py

clean:
	rm -f $(TARGET)
