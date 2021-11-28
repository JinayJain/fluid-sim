CXX := g++
INCLUDE := -Iinclude

SOURCE := src
BUILD := build

TARGET := main
DEPS := $(BUILD)/main.o $(BUILD)/sim.o

$(TARGET): $(DEPS)
	$(CXX) $^ -o $@

$(BUILD)/%.o: $(SOURCE)/%.cpp
	$(CXX) $(INCLUDE) -c -o $@ $<

clean:
	rm -f $(BUILD)/*

run: $(TARGET)
	./$(TARGET)
