#ifndef _UTIL_H
#define _UTIL_H

static char isdigit(char x) {
	return x=='0' || x=='1' || x=='2' || x=='3' ||
	       x=='4' || x=='5' || x=='6' || x=='7' ||
	       x=='8' || x=='9';
}
static int digit2int(char x) { return x - '0'; }

#endif
