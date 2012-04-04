CXX = g++

LDFLAGS = -g -I../inc -L. -Wl,-rpath=../ ../libtsal.so.0

SOURCES = src/source.cpp src/global.cpp

EXECUTABLES = $(patsubst src/%.cpp, %, $(SOURCES))

all: $(EXECUTABLES)

% : src/%.cpp
	$(CXX) $(LDFLAGS) $< -o $@

clean:
	$(RM) $(EXECUTABLES)
