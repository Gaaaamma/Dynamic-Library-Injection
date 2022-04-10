CC = gcc
CXX = g++
CFLAGS = -Wall -g
filename = hw2.c
LIB = logger.so
LIBFLAGS = -shared -fPIC -ldl

.PHONY: all clean

all: $(LIB) functionTester

$(LIB): $(filename)
	$(CC) -o $@ $< $(LIBFLAGS)

functionTester: functionTester.c
	$(CC) $(CFLAGS) $< -o $@

clean:  
	rm -f functionTester
	rm -f $(LIB)
	rm -f aaa.txt ;
	rm -f bbb.txt ; 
	rm -f ccc.txt ;
