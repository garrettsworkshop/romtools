#ifndef _UTIL_H
#define _UTIL_H

static int quit(int x) { exit(x); return x; }

static int quit_infile(char *filename) {
	fprintf(stderr, "romswap: ERROR! Failed to open input file \"%s\"!\n", filename);
	return quit(-1);
}


static int quit_outfile(char *filename) {
	fprintf(stderr, "romswap: ERROR! Failed to open output file \"%s\"!\n", filename);
	return quit(-1);
}

static char isdigit(char x) {
	return x=='0' || x=='1' || x=='2' || x=='3' ||
	       x=='4' || x=='5' || x=='6' || x=='7' ||
	       x=='8' || x=='9';
}
static int digit2int(char x) { return x - '0'; }

#endif
