#CXX = g++ 
#CXXFLAGS = -O3 -pipe -fomit-frame-pointer -ftracer 
#Uncomment the lines above, if you don't use the gentoo ebuild (and maybe optimize them for your system)


LD = $(CXX)
LDFLAGS += -lpthread

OBJ = Benchmark.o Stopwatch.o Sysinfo.o Write_Result.o md5.o main.o
BIN = bashmark

$(BIN):  $(OBJ)
	$(LD) $(LDFLAGS) $(CXXFLAGS) $(OBJ) -o $(BIN)


Benchmark.o:
	$(CXX) $(CXXFLAGS) -c Benchmark.cpp -o Benchmark.o

Stopwatch.o:
	$(CXX) $(CXXFLAGS) -c Stopwatch.cpp -o Stopwatch.o 

Sysinfo.o:
	$(CXX) $(CXXFLAGS) -c Sysinfo.cpp -o Sysinfo.o

Write_Result.o:
	$(CXX) $(CXXFLAGS) -c Write_Result.cpp -o Write_Result.o

md5.o:
	$(CXX) $(CXXFLAGS) -c md5.c -o md5.o

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o


install:
	cp -f $(BIN) /usr/bin/

.PHONY:
clean:	
	rm -f $(BIN)
	rm -f $(OBJ)
