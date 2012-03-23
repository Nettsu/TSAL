CC = g++
CCWIN = i486-mingw32-g++

CFLAGS = -O2 -Wall -c -fPIC -I. -I./dep/inc -I./inc
CFLAGSWIN = -O2 -Wall -c -DBUILDING_EXAMPLE_DLL -I. -I./dep/inc -I./inc

DBGFLAGS = -g

LDFLAGS = -O2 -L./dep/linux -lopenal -Wl,-soname,libtsal.so.0,--whole-archive \
-logg -lvorbis -lvorbisfile -Wl,--no-whole-archive -shared

LDFLAGSWIN = -O2 -L./dep/win32 -static-libgcc -static-libstdc++ -lsoft_oal -Wl,--whole-archive \
-logg -lvorbis -lvorbisfile -Wl,--no-whole-archive -shared

SOURCES = src/source.cpp src/mixer.cpp src/oal_wrap.cpp

NODIRSOURCES = $(notdir $(SOURCES))

OBJECTS = $(patsubst %.cpp, src/%.o, $(NODIRSOURCES))
OBJECTSWIN = $(patsubst %.cpp, src/%.win.o, $(NODIRSOURCES))

EXECUTABLE = lib/libtsal.so.0
EXECUTABLEWIN = lib/libtsal.dll

all: $(SOURCES) $(EXECUTABLE) 

win32 : $(SOURCES) $(EXECUTABLEWIN)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(DBGFLAGS) $(OBJECTS) -o $@

$(EXECUTABLEWIN): $(OBJECTSWIN)
	$(CCWIN) $(LDFLAGSWIN) $(DBGFLAGS) $(OBJECTSWIN) -o $@

src/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

src/%.win.o: src/%.cpp
	$(CCWIN) $(CFLAGSWIN) $(DBGFLAGS) $< -o $@

clean:
	$(RM) src/*.o
