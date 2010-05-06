OBJS=

default: test


urlparser.o: urlparser.c urlparser.h
	gcc -Wall -pedantic -g -c -o $@ $<
OBJS+=urlparser.o

test: test.c urlparser.o
	gcc -Wall -pedantic -g -o $@ $< $(OBJS)

clean:
	rm -f  test $(OBJS)
