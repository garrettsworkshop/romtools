#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef uint8_t data_t;
data_t d_map[256];
uint8_t d_bits;
int8_t d_map_pattern[8];

typedef uint16_t addr_t;
addr_t a_map[65536];
uint8_t a_bits;
int8_t a_map_pattern[16];

data_t buf[65536];

int hex2int(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	fprintf(stderr, "romswap: ERROR! Non-hex character \'%c\' in map!", c);
	exit(1);
}

// romswap FEDCBA9876543210 76543210 file.bin
int main (int argc, char *argv[]) {
	if (argc != 5) {
		fprintf(stderr, "usage: %s <address map> <data map> <input file> <output file>\n", argv[0]);
		exit(1);
	}

	a_bits = strlen(argv[1]);
	if (a_bits > 16) {
		fprintf(stderr, "romswap: ERROR! Must have no more than 16 bits in address map!\n");
		exit(1);
	}
	for (int i = 0; i < a_bits; i++) {
		a_map_pattern[i] = hex2int(argv[1][i]);
	}
	for (int i = a_bits; i < 16; i++) {
		a_map_pattern[i] = i;
	}
	for (int i = 0; i < 65535; i++) {
		addr_t a = 0;
		for (int j = 0; j < a_bits; j++) {
			a <<= 1;
			a |= (i >> a_map_pattern[j]) & 1;
		}
		a_map[i] = a;
	}

	d_bits = strlen(argv[2]);
	if (d_bits != 8) {
		fprintf(stderr, "romswap: ERROR! Must have 8 bits in data map!\n");
		exit(1);
	}
	for (int i = 0; i < d_bits; i++) { d_map_pattern[i] = hex2int(argv[2][i]); }
	for (int i = 0; i < 255; i++) {
		data_t d = 0;
		for (int j = 0; j < d_bits; j++) {
			d <<= 1;
			d |= (i >> d_map_pattern[j]) & 1;
		}
		d_map[i] = d;
	}

	FILE *in = fopen(argv[3], "r");
	if (in == NULL) { 
		fprintf(stderr, "romswap: ERROR! Failed to open input file! \"%s\"\n", argv[3]);
		exit(1);
	}
	FILE *out = fopen(argv[4], "w+");
	if (out == NULL) { 
		fprintf(stderr, "romswap: ERROR! Failed to open input file \"%s\"!\n", argv[4]);
		exit(1);
	}

	while (!feof(in)) {
		size_t count = fread(buf, sizeof(uint8_t), sizeof(buf), in);
		for (int i = 0; i < count; i++) {
			fputc(d_map[buf[a_map[i]]], out);
		}
	}

	fclose(out);
	fclose(in);
}
