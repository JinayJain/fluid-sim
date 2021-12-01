CXX := nvc++
INCLUDE := -Iinclude
CPPFLAGS := -acc -gpu=cc70 -Minfo=accel
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
