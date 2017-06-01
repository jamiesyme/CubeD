CXX=g++

CUBED_LIBS=sfml-system sfml-window sfml-graphics gl
CUBED_CXXFLAGS=-Wall -std=c++17 $(shell pkg-config --cflags $(CUBED_LIBS))
CUBED_LDFLAGS=$(shell pkg-config --libs $(CUBED_LIBS))
CUBED_SOURCES=$(wildcard src/*.cpp)
CUBED_BINARY=cubed

all: cubed

cubed: $(CUBED_SOURCES)
	@$(CXX) $(CUBED_CXXFLAGS) $(CUBED_SOURCES) $(CUBED_LDFLAGS) -o $(CUBED_BINARY)

clean:
	@rm -f $(CUBED_BINARY)
