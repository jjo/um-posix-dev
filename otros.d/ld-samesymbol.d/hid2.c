//int misma_func () __attribute__ ((weak));
int misma_func(void) {
	return 2;
}
int func2(void) {
	return misma_func();
}

