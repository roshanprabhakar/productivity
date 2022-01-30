gcc -Wall -g -c src/constants.c -o lib/obj/constants.o
gcc -Wall -g -c src/goals.c -o lib/obj/goals.o
gcc -Wall -g -c src/gutils.c -o lib/obj/gutils.o
gcc -Wall -g -c src/global_test.c -o lib/obj/global_test.o
gcc -Wall -g -c src/goals_test.c -o lib/obj/goals_test.o
gcc -Wall -g -c src/gutils_test.c -o lib/obj/gutils_test.o

gcc lib/obj/global_test.o lib/obj/constants.o -o lib/build/global_test
gcc lib/obj/gutils_test.o lib/obj/gutils.o lib/obj/constants.o lib/obj/goals.o -o lib/build/gutils_test
gcc lib/obj/goals_test.o lib/obj/goals.o lib/obj/constants.o -o lib/build/goals_test