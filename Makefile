flags=-O2 -Wall -std=c2x

.PHONY: all clean

all: clean birdflu

birdflu: birdflu.o
	cc $(flags) $^ -o $@ $(ldflags)

birdflu.o: birdflu.c birdflu.h
	cc $(flags) -c $<

clean:
	rm -f *.o birdflu