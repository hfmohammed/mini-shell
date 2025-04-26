# compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wno-c++11-extensions

# target output
TARGET = main

# compiler and run
run: $(TARGET)
	./main

# compile the program
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

# clean the directory
clean:
	rm -f $(TARGET)
