#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "gutils.h"
#include "constants.h"

void unit_extract_value() {
	printf("\nextract_value {\n");
	char* earg = "gunit*-ca*50*-rb*-s*this is a string*-da*50*-sp*1*-d*0.5*-l*39393939939393";
	printf("extracting from: %s\n", earg);

	int argc;
	char** args = getargv(earg, &argc, "*");
	int* val1 = (int*) extract_value(argc, args, "-ca", 'i');
	printf("-ca: %d\n", *val1);
	free(val1);

	bool* val2 = (bool*) extract_value(argc, args, "-rb", 'b');
	printf("-rb: %d\n", *val2);
	free(val2);
	
	char* val3 = (char*) extract_value(argc, args, "-s", 's');
	printf("-s: %s\n", val3);
	free(val3);
	
	int* val4 = (int*) extract_value(argc, args, "-da", 'i');
	printf("-da: %d\n", *val4);
	free(val4);
	
	int* val5 = (int*) extract_value(argc, args, "-sp", 'i');
	printf("-sp: %d\n", *val4);
	free(val5);
	
	int* nonex = (int*) extract_value(argc, args, "-nonex", 'i');
	printf("should be null: %p\n", nonex);

	double* val6 = (double*) extract_value(argc, args, "-d", 'd');
	printf("-d: %lf\n", *val6);
	free(val6);

	long* val7 = (long*) extract_value(argc, args, "-l", 'l');
	printf("-l: %ld\n", *val7);
	free(val7);
	
	for (int i = 0; i < argc; i++) {
		free(args[i]);
	}
	free(args);
	
	printf("}\n");
}

void unit_get_argv() {
	printf("\nget_arv {\n");
	char* earg = "gunit*-ca*50*-rb*-s*this is a string*-da*50*-sp*1";
	printf("extracting from: %s\n", earg);
	int argc;
	char** args = getargv(earg, &argc, "*");
	printf("argc: %d\n", argc);
	for (int i = 0; i < argc; i++) {
		printf("%d: %s\n", i, args[i]);
		free(args[i]);
	}
	free(args);
	printf("}\n");
}

int main() {
    unit_get_argv();
	unit_extract_value();
    return 0;
}