PROGRAM = s2launch
BINARY = $(PROGRAM).exe

PHOENIX = /mingw/include/phoenix/phoenix.o
PHOENIXRES = /mingw/include/phoenix/windows/phoenix-resource.o

CXXFLAGS = -std=gnu++0x -D__MSVCRT_VERSION__=0x0601
LDFLAGS = -lkernel32 -luser32 -lgdi32 -ladvapi32 -lcomctl32 -lcomdlg32 -lshell32 -lole32

.PHONY: all clean

all: $(BINARY)

clean:
	rm -f *.o
	rm -f $(BINARY)

$(BINARY): $(PROGRAM).o
	$(CXX) -mwindows -s -static-libgcc -static-libstdc++ $+ $(PHOENIX) $(PHOENIXRES) $(LDFLAGS) -o $@