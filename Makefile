CXX = g++
CFLAGS = -std=c++11 -Wall -g
filename = hw2.cpp
outputFile = hw2

.PHONY: clean

$(outputFile): $(filename)
	$(CXX) $(CFLAGS) $(filename) -o $(outputFile)
	
clean:  
	rm -f $(outputFile)
