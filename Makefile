CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra
LDLIBS ?= -lncurses

TARGET = snake_game
SOURCES = main.cpp board.cpp snake_logic.cpp item_score.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDLIBS) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
