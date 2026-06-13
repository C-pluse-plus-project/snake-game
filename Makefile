CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra
LDLIBS ?= -lncurses

TARGET = snake_game
SOURCES = main.cpp game_manager.cpp board.cpp snake.cpp item_score.cpp gate.cpp food.cpp poison.cpp wall.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDLIBS) -o $(TARGET)

.PHONY: clean run

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
