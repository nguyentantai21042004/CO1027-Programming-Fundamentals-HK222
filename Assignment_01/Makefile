CXX = g++
CXXFLAGS = -Wall -Wextra
VPATH = build

all: build/bin/run

build/bin/run: main.o knight.o
	mkdir -p build/bin
	$(CXX) $(CXXFLAGS) $^ -o $@

build/main.o: main.cpp main.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/knight.o: knight.cpp knight.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rvf build

.PHONY: run
run: build/bin/run
	./build/bin/run
