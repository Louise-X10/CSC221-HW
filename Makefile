CXX=clang++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17 -O0 -g -fsanitize=address
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)

all:  tsp test_chromosome

tsp: tsp.o chromosome.o deme.o cities.o
	$(CXX) $(LDFLAGS) -o $@ $^

test_chromosome: test_chromosome.o cities.o chromosome.o 
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o tsp test_chromosome

test: test_chromosome
	./test_chromosome
