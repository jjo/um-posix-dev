#include <stdio.h>

int func(char *buf) {
	buf[0]='a';
	return 1;
}
int main (int argc, char * argv[]) {
	static char buffer[256];
	func(buffer);
	return 0;
}
