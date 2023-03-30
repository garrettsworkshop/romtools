#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

static int quit(int x) { exit(x); return x; }

static int quit_infile(char *filename) {
	fprintf(stderr, "bytesplit: ERROR! Failed to open input file \"%s\"!\n", filename);
	return quit(-1);
}


static int quit_outfile(char *filename) {
	fprintf(stderr, "bytesplit: ERROR! Failed to open output file \"%s\"!\n", filename);
	return quit(-1);
}

static int quit_usage() {
	fprintf(stderr, "usage: bytesplit <input_file> <interleave_factor> [<output_files>]\n");
	return quit(-1);
}

static int quit_interleave() {
	fprintf(stderr, "bytesplit: ERROR! Interleave factor must be 2, 4, or 8.\n");
	return quit(-1);
}

static int warn_length_divisibility() {
	fprintf(stderr, "bytesplit: WARNING! Input file length not divisible by interleave factor.\n");
	return quit(-1);
}

// bytesplit input.bin 2 output_even.bin output_odd.bin
int main (int argc, char *argv[]) {
	// Check argument count must be 5 or more (for interleave factor 2)
	if (argc < 5) { return quit_usage(); }

	// Open input file
	FILE *infile = fopen(argv[1], "r");
	if (infile == NULL) { return quit_infile(argv[1]); }

	// Check interleave factor argument
	if ((!isdigit(argv[2][0])) || // Check 1st char is digit
		(argv[2][1] != '\0')) { // Check 2nd char is null
		return quit_usage();
	}
	int interleave_factor = digit2int(argv[2][0]); // Convert string to digit
	switch(interleave_factor) { // Check interleave factor is 2, 4, or 8
		case 2: case 4: case 8: break;
		default: return quit_interleave();
	}

	// Check argument count against interleave factor
	if (argc != (3 + interleave_factor)) { return quit_usage(); }

	// Open output files
	FILE *outfiles[8] = {0};
	for (int i = 0; i < interleave_factor; i++) {
		outfiles[i] = fopen(argv[3 + i], "w+");
		if (outfiles[i] == NULL) { return quit_outfile(argv[3 + i]); }
	}

	// Read input and write every other byte to output
	int c = fgetc(infile);
	while (c != EOF) {
		for (int i = 0; i < interleave_factor; i++) {
			if (c != EOF) {
				fputc(c, outfiles[i]);
				c = fgetc(infile);
			} else {
				// file length not divisible by interleave factor
				warn_length_divisibility();
			}
		}
	}

	// Close input and output files
	fclose(infile);
	for (int i = 0; i < 8; i++) {
		if (outfiles[i] != NULL) { fclose(outfiles[i]); }
	}
}
