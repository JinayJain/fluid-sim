CXX := nvc++
CPPFLAGS := -lsfml-window -lsfml-graphics -lsfml-system -acc -ta=multicore -Minfo=accel 
INCLUDE := -Iinclude

SOURCE := src
BUILD := build

TARGET := main
DEPS := $(BUILD)/main.o $(BUILD)/sim.o

$(TARGET): $(DEPS)
	$(CXX) $^ $(CPPFLAGS) -o $@

$(BUILD)/%.o: $(SOURCE)/%.cpp
	$(CXX) $(CPPFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	rm -f $(BUILD)/*

run: $(TARGET)
	./$(TARGET)
