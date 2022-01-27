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
	char** args = getargv(earg, 14);
	
	int* val1 = (int*) extract_value(14, args, "-ca", 'i');
	printf("-ca: %d\n", *val1);
	free(val1);

	bool* val2 = (bool*) extract_value(14, args, "-rb", 'b');
	printf("-rb: %d\n", *val2);
	free(val2);
	
	char* val3 = (char*) extract_value(14, args, "-s", 's');
	printf("-s: %s\n", val3);
	free(val3);
	
	int* val4 = (int*) extract_value(14, args, "-da", 'i');
	printf("-da: %d\n", *val4);
	free(val4);
	
	int* val5 = (int*) extract_value(14, args, "-sp", 'i');
	printf("-sp: %d\n", *val4);
	free(val5);
	
	int* nonex = (int*) extract_value(14, args, "-nonex", 'i');
	printf("should be null: %p\n", nonex);

	double* val6 = (double*) extract_value(14, args, "-d", 'd');
	printf("-d: %lf\n", *val6);
	free(val6);

	long* val7 = (long*) extract_value(14, args, "-l", 'l');
	printf("-l: %ld\n", *val7);
	free(val7);
	
	for (int i = 0; i < 14; i++) {
		free(args[i]);
	}
	free(args);
	
	printf("}\n");
}

void unit_get_argv() {
	printf("\nget_arv {\n");
	char* earg = "gunit*-ca*50*-rb*-s*this is a string*-da*50*-sp*1";
	printf("extracting from: %s\n", earg);
	char** args = getargv(earg, 10);
	for (int i = 0; i < 10; i++) {
		printf("%d: %s\n", i, args[i]);
		free(args[i]);
	}
	free(args);
	printf("}\n");
}

int main() {
    unit_extract_value();
    unit_get_argv();
    return 0;
}