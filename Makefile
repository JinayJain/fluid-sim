CXX := g++
CPPFLAGS := -lsfml-window -lsfml-graphics -lsfml-system
INCLUDE := -Iinclude

SOURCE := src
BUILD := build

TARGET := main
DEPS := $(BUILD)/main.o $(BUILD)/sim.o

$(TARGET): $(DEPS)
	$(CXX) $^ $(CPPFLAGS) -o $@

$(BUILD)/%.o: $(SOURCE)/%.cpp
	$(CXX) $(INCLUDE) -c -o $@ $<

clean:
	rm -f $(BUILD)/*

run: $(TARGET)
	./$(TARGET)
