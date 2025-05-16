CXX      := g++
CXXFLAGS := -std=c++17 -Iinclude -O2 -Wall

SRC      := $(wildcard src/*.cpp)
OBJ      := $(SRC:.cpp=.o)
TARGET   := retail_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q src\*.o $(TARGET).exe

.PHONY: all clean
