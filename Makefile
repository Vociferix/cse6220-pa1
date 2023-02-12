all: int_calc

.PHONY: all

int_calc: main.cpp
	mpic++ -o int_calc main.cpp -O3 -DNDEBUG

clean:
	rm -f int_calc

.PHONY: clean
