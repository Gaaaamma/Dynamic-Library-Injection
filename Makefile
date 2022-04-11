CC = gcc
CXX = g++
CFLAGS = -Wall -g
filename = hw2.c
LIB = logger.so
LIBFLAGS = -shared -fPIC -ldl

.PHONY: all clean

all: $(LIB) logger 

$(LIB): $(filename)
	$(CC) -o $@ $< $(LIBFLAGS)

logger: logger.c
	$(CC) -o $@ $< $(CFLAGS)

functionTester: functionTester.c
	$(CC) $(CFLAGS) $< -o $@

clean:  
	rm -f functionTester
	rm -f logger
	rm -f $(LIB)
	rm -f aaa.txt ;
	rm -f bbb.txt ; 
	rm -f ccc.txt ;
