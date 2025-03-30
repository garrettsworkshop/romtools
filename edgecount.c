#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

static int quit(int x) { exit(x); return x; }

static int quit_infile(char *filename) {
	fprintf(stderr, "edgecount: ERROR! Failed to open input file \"%s\"!\n", filename);
	return quit(-1);
}

static int quit_usage() {
	fprintf(stderr, "usage: edgecount <input file>\n");
	return quit(-1);
}

// edgecount input.bin
int main (int argc, char *argv[]) {
	// Total edges
	long edges = 0;

	// Check argument count
	if (argc != 2) { return quit_usage(); }

	// Open input file
	FILE *infile = fopen(argv[1], "r");
	if (infile == NULL) { return quit_infile(argv[3]); }

	int last = 0;
	int c = fgetc(infile);
	while (c != EOF) {
		int8_t b = c;
		for (int i = 7; i >= 0; i--) {
			b = (c >> i) & 1;
			if (b != last) { edges++; }
			last = b;
		}
		c = fgetc(infile);
	}

	fclose(infile);

	printf("Total edges: %li\n", edges);
}
