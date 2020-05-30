CXX = gcc

FLAGS = -Wall -Werror -std=c99

CFLAGS = -c -Wall -Werror -std=c99

LIBS = lib/libmemory.a lib/libmyTerm.a lib/libmyBigChars.a lib/libPrintAll.a lib/libmyReadkey.a lib/libtransGeomAdd.a lib/libcpu.a

all: makedirs bin/key 
	rm build/*.o

bin/key: build/main.o $(LIBS)
	$(CXX) $(FLAGS) build/main.o $(LIBS) -o bin/key

build/main.o: src/main.c
	$(CXX) $(CFLAGS) src/main.c -o build/main.o

build/cpu.o: src/cpu.c
	$(CXX) $(CFLAGS) src/cpu.c -o build/cpu.o

lib/libcpu.a: build/cpu.o
	ar cr lib/libcpu.a build/cpu.o

build/transGeomAdd.o: src/transGeomAdd.c
	$(CXX) $(CFLAGS) src/transGeomAdd.c -o build/transGeomAdd.o

lib/libtransGeomAdd.a: build/transGeomAdd.o
	ar cr lib/libtransGeomAdd.a build/transGeomAdd.o

build/myReadkey.o: src/myReadkey.c
	$(CXX) $(CFLAGS) src/myReadkey.c -o build/myReadkey.o

lib/libmyReadkey.a: build/myReadkey.o
	ar cr lib/libmyReadkey.a build/myReadkey.o

build/myBigChars.o: src/myBigChars.c
	$(CXX) $(CFLAGS) src/myBigChars.c -o build/myBigChars.o

lib/libmyBigChars.a: build/myBigChars.o
	ar cr lib/libmyBigChars.a build/myBigChars.o

build/myTerm.o: src/myTerm.c
	$(CXX) $(CFLAGS) src/myTerm.c -o build/myTerm.o

lib/libmyTerm.a: build/myTerm.o
	ar cr lib/libmyTerm.a build/myTerm.o

build/memory.o: src/memory.c
	$(CXX) $(CFLAGS) src/memory.c -o build/memory.o

lib/libmemory.a: build/memory.o
	ar cr lib/libmemory.a build/memory.o

build/PrintAll.o: src/PrintAll.c
	$(CXX) $(CFLAGS) src/PrintAll.c -o build/PrintAll.o

lib/libPrintAll.a: build/PrintAll.o
	ar cr lib/libPrintAll.a build/PrintAll.o

makedirs:
	mkdir build -p
	mkdir bin -p
	mkdir lib -p

clean:
	rm -rf build
	rm -rf bin
	rm -rf lib
