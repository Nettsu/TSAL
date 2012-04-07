CXX = g++-4.3
CXX_WIN = i486-mingw32-g++

CFLAGS = -O2 -Wall -c -fPIC -I. -I./dep/inc -I./inc
CFLAGS_WIN = -O2 -Wall -c -DBUILDING_EXAMPLE_DLL -I. -I./dep/inc -I./inc

ARCH = 32

LIBPATH = -L./dep/linux32
ifeq "$(ARCH)" "64"
LIBPATH = -L./dep/linux64
endif

DBGFLAGS = -g

LDFLAGS = -O2 $(LIBPATH) -lrt -Wl,-soname,libtsal.so.0,--whole-archive \
-lopenal -logg -lvorbis -ldl -lvorbisfile -Wl,--no-whole-archive -shared

LDFLAGS_WIN = -O2 -L./dep/win32 -static-libgcc -static-libstdc++ -lsoft_oal \
-Wl,--whole-archive -logg -lvorbis -lvorbisfile -Wl,--no-whole-archive -shared

SOURCES = src/source.cpp src/mixer.cpp src/oal_wrap.cpp src/priv_source.cpp

NODIRSOURCES = $(notdir $(SOURCES))

OBJECTS_LIN = $(patsubst %.cpp, src/%.o, $(NODIRSOURCES))
OBJECTS_WIN = $(patsubst %.cpp, src/%.win.o, $(NODIRSOURCES))

EXECUTABLE_LIN = libtsal.so.0
EXECUTABLE_WIN = libtsal.dll

all: $(SOURCES) $(EXECUTABLE_LIN) 

win32 : $(SOURCES) $(EXECUTABLE_WIN)

$(EXECUTABLE_LIN): $(OBJECTS_LIN)
	$(CXX) $(LDFLAGS) $(DBGFLAGS) $(OBJECTS_LIN) -o $@

$(EXECUTABLE_WIN): $(OBJECTS_WIN)
	$(CXX_WIN) $(LDFLAGS_WIN) $(DBGFLAGS) $(OBJECTS_WIN) -o $@

src/%.o: src/%.cpp
	$(CXX) $(CFLAGS) $(DBGFLAGS) $< -o $@

src/%.win.o: src/%.cpp
	$(CXX_WIN) $(CFLAGS_WIN) $(DBGFLAGS) $< -o $@

clean:
	$(RM) src/*.o
