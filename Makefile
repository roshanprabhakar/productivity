# Link
lib/build/unit_tests: lib/bin/goals.o lib/bin/gunit.o
	gcc lib/bin/goals.o lib/bin/gunit.o -o lib/build/unit_tests

# compile
lib/bin/goals.o: src/goals.c
	gcc -Wall -g -c src/goals.c -o lib/bin/goals.o

lib/bin/gunit.o: src/gunit.c
	gcc -Wall -g -c src/gunit.c -o lib/bin/gunit.o
