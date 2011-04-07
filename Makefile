CXX=g++
CC=g++
CXXFLAGS=-Wall
LDFLAGS=
BINDIR=/usr/local/bin
MANDIR=/usr/share/man/man1
OWNER=bin
GROUP=bin

all: src/dupfinder

src/dupfinder: src/dupfinder.o src/file.o
	g++ -lboost_filesystem src/dupfinder.o src/file.o -o src/dupfinder

src/dupfinder.o: src/dupfinder.cpp src/vectormap.hpp src/file.hpp src/config.hpp

src/file.o: src/file.hpp src/file.cpp

clean:
	rm -f src/dupfinder.o src/file.o src/dupfinder

install: src/dupfinder
	install -c -o ${OWNER} -g ${GROUP} -m 755 src/dupfinder ${BINDIR}
	install -c -o ${OWNER} -g ${GROUP} -m 644 man/dupfinder.man ${MANDIR}/dupfinder.1

