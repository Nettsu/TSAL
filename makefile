CC = g++
CCWIN = i486-mingw32-g++

CFLAGS = -O2 -Wall -c -fPIC -I. -I./dep/inc -I./inc
CFLAGSWIN = -O2 -Wall -c -DBUILDING_EXAMPLE_DLL -I. -I./dep/inc -I./inc

DBGFLAGS = -g

DEPDIR32 = -L./dep/linux32
DEPDIR64 = -L./dep/linux64
DEPDIRWIN = -L./dep/win32

LDFLAGS = -O2 -lopenal -Wl,-soname,libtsal.so.0,--whole-archive \
-logg -lvorbis -lvorbisfile -Wl,--no-whole-archive -shared

LDFLAGSWIN = -O2 -static-libgcc -static-libstdc++ -lsoft_oal -Wl,--whole-archive \
-logg -lvorbis -lvorbisfile -Wl,--no-whole-archive -shared

SOURCES = src/source.cpp src/mixer.cpp src/oal_wrap.cpp

NODIRSOURCES = $(notdir $(SOURCES))

OBJECTS = $(patsubst %.cpp, src/%.o, $(NODIRSOURCES))
OBJECTSWIN = $(patsubst %.cpp, src/%.win.o, $(NODIRSOURCES))

EXECUTABLE = lib/libtsal.so.0
EXECUTABLEWIN = lib/libtsal.dll

all: $(SOURCES) $(EXECUTABLE) 

win32 : $(SOURCES) $(EXECUTABLEWIN)

linux64: $(SOURCES) $(EXECUTABLE64)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(DEPDIR32) $(LDFLAGS) $(DBGFLAGS) $(OBJECTS) -o $@

$(EXECUTABLE64): $(OBJECTS)
	$(CC) $(DEPDIR64) $(LDFLAGS) $(DBGFLAGS) $(OBJECTS) -o $@

$(EXECUTABLEWIN): $(OBJECTSWIN)
	$(CCWIN) $(DEPDIRWIN) $(LDFLAGSWIN) $(DBGFLAGS) $(OBJECTSWIN) -o $@

src/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

src/%.win.o: src/%.cpp
	$(CCWIN) $(CFLAGSWIN) $(DBGFLAGS) $< -o $@

clean:
	$(RM) src/*.o
