//int misma_func () __attribute__ ((weak));
int misma_func(void) {
	return 1;
}
int func1(void) {
	return misma_func();
}
