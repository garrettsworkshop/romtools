#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int quit(int x) { exit(x); return x; }

static char isdigit(char x) {
	return x=='0' || x=='1' || x=='2' || x=='3' ||
	       x=='4' || x=='5' || x=='6' || x=='7' ||
	       x=='8' || x=='9';
}
static int digit2int(char x) { return x - '0'; }

static int quit_usage() {
	fprintf(stderr, "usage: romswap <address map> <data map> <input file> <output file>\n");
	return quit(-1);
}

static int quit_bitcount() {
	fprintf(stderr, "romswap: ERROR! Too many bits in map pattern.\n");
	return quit(-1);
}

static int quit_databitcount() {
	fprintf(stderr, "romswap: ERROR! Too many bits in data map pattern.\n");
	return quit(-1);
}

static int quit_bitindex() {
	fprintf(stderr, "romswap: ERROR! Too high bit index.\n");
	return quit(-1);
}

static int quit_infile(char *filename) {
	fprintf(stderr, "romswap: ERROR! Failed to open input file \"%s\"!\n", filename);
	return quit(-1);
}


static int quit_outfile(char *filename) {
	fprintf(stderr, "romswap: ERROR! Failed to open output file \"%s\"!\n", filename);
	return quit(-1);
}

int read_pin_list(char *map_str, int *map_pattern, int maxbits) {
	int map_str_length = strlen(map_str);

	int bits = 0;
	char cur = 0;
	char last = 0;
	char last2 = 0;
	for (int i = 0; i < map_str_length; i++) {
		last2 = last;
		last = cur;
		cur = map_str[i];

		// Fail if too many pin bits
		if (i >= maxbits) { return quit_bitcount(); }

		// Fail if not digit or comma
		if (!isdigit(cur) && (cur != ',')) { return quit_usage(); }

		// Fail if two consecutive commas
		if ((last == ',') && (cur == ',')) { return quit_usage(); }

		// Fail if three consecutive digits
		if (isdigit(last2) && isdigit(last) && isdigit(cur)) { return quit_bitindex(); }

		// Fail if first char not digit
		if (i==0 && !isdigit(cur)) { return quit_usage(); }

		// Submit previous digits 
		if (i > 0 && ((cur == ',') || (i == map_str_length - 1))) {
			int pin;
			if (cur == ',') {
				pin = digit2int(last);
				if (isdigit(last2)) { pin += 10 * digit2int(last2); }
			} else {
				pin = digit2int(cur);
				if (isdigit(last)) { pin += 10 * digit2int(last); }
			}

			if (pin >= maxbits) { return quit_bitindex(); }

			map_pattern[bits] = pin;
			bits++;
		}
	}

	return bits;
}

static void gen_map(int bits, int *pattern, int *map) {
	int size = pow(2, bits); // Map size is 2^n
	// Iterate through entire map
	for (int i = 0; i < size; i++) {
		int entry = 0;
		// Iterate through each bit in the entry
		for (int j = 0; j < bits; j++) {
			// Get jth bit according to pattern
			int bit = (i >> pattern[j]) & 1;
			// Shift entry left and add in jth bit
			entry = (entry << 1) | bit;
		}
		map[i] = entry;
	}
}

// romswap 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 0,1,2,3,4,5,6,7 input.bin output.bin
int main (int argc, char *argv[]) {
	if (argc != 5) { return quit_usage(); }

	// Generate address swap map
	#define MAX_ADDR_BITS (20)
	int a_map_pattern[MAX_ADDR_BITS];
	int a_bits = read_pin_list(argv[1], a_map_pattern, MAX_ADDR_BITS);
	if (a_bits < 1) { return quit_usage(); }
	int rom_size = pow(2, a_bits);
	int a_map[rom_size];
	gen_map(a_bits, a_map_pattern, a_map);

	// Generate data swap map
	#define DATA_BITS (8)
	#define DATA_MAP_SIZE (256)
	int d_map_pattern[DATA_BITS];
	int d_bits = read_pin_list(argv[2], d_map_pattern, DATA_BITS);
	if (d_bits != DATA_BITS) { return quit_databitcount(); }
	int d_map[DATA_MAP_SIZE];
	gen_map(d_bits, d_map_pattern, d_map);

	// Open input and output files
	FILE *infile = fopen(argv[3], "r");
	if (infile == NULL) { return quit_infile(argv[3]); }
	FILE *outfile = fopen(argv[4], "w+");
	if (outfile == NULL) { return quit_outfile(argv[4]); }

	// Read input and do data swap
	char buf[rom_size];
	for (int i = 0; i < rom_size; i++) {
		int c = fgetc(infile);
		if (c == EOF) { break; }
		buf[i] = d_map[c];
	}
	fclose(infile);

	// Write output with address swap
	for (int i = 0; i < rom_size; i++) {
		fputc(buf[a_map[i]], outfile);
	}
	fclose(outfile);
}
