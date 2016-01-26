CFLAGS=-g -std=c++11 -Wall
OPT=0
ifeq ($(OPT),1)
CFLAGS+=-O3
endif

all: *.cpp *.h
	g++ $(CFLAGS) *.cpp

clean:
	rm -f a.out
