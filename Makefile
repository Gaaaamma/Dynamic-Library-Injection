cc= g++ -std=c++11 
filename = hw2.cpp
outputFile = hw2

all: hw2.cpp
	$(cc) $(filename) -o $(outputFile)

clean:
	rm -f $(outputFile)
