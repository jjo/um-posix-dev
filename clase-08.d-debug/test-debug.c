#include <stdio.h>
#include <stdlib.h>

int func(int *ptr, int len) {
	int i;
	for (i=0;i<len;i++)
		ptr[i]=i;
	return 1;
}
int main (void) {
	int *array_i=malloc(sizeof (int) * 10);
	func(array_i, 50);
	return 0;
}
