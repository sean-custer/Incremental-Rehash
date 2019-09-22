CXX = g++
CXXFLAGS = -Wall -g

test2.out: HashTable.o test2.cpp primes.o
	$(CXX) $(CXXFLAGS) HashTable.o primes.o test2.cpp -o run

HashTable.o: HashTable.cpp HashTable.h
	$(CXX) $(CXXFLAGS) HashTable.cpp HashTable.h -c

primes.o: primes.cpp primes.h
	$(CXX) $(CXXFLAGS) primes.cpp primes.h -c

clean:
	rm *.o output


