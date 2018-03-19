ifeq ($(CC),g++)
  FLAGS=-O3 -march=native -std=c++11
#  FLAGS=-O3 -Wall -g -ggdb
  DFLAGS=-ggdb -std=c++11
endif

ifeq ($(CC),icpc)
  FLAGS=-fast -msse4.2 -ipo -std=c++11
  DFLAGS=-ggdb -std=c++11
endif

SRC=$(patsubst source/%, %, $(wildcard source/*.cpp))
OBJ=$(patsubst %.cpp, release/%.o, ${SRC})
DOBJ=$(patsubst %.cpp, debug/%.o, ${SRC})

all: dir $(OBJ)
        @echo -n "[Linking] $@... "
#       @ar rcs lib/x64_linux/static/libCVRPSep.a $(OBJ)
        g++ $(FLAGS) -o gencarp $(OBJ)
        @echo "done!"

debug: dir-debug $(DOBJ)
        @echo -n "[Linking debug] $@..."
        @ar rcs lib/x64_linux/static_debug/libCVRPSep.a $(DOBJ)
        @echo "done!"

obj: $(OBJ)

dobj: $(DOBJ)

release/%.o: source/%.cpp
        @echo -n "[Compiling] $@... "
        @$(CC) -c -o $@ $< $(FLAGS) -I./include
        @echo "done!"

debug/%.o: source/%.cpp
        @echo -n "[Compiling debug] $@... "
        @$(CC) -c -o $@ $< $(DFLAGS) -I./include
        @echo "done!"

clean:
        @echo -n "[Cleaning] release debug... "
        -@rm -rf release debug 2>/dev/null || true
        @echo "done!"

dir:
        @mkdir -p release
        @mkdir -p lib/x64_linux/static

dir-debug:
        @mkdir -p debug
        @mkdir -p lib/x64_linux/static_debug
