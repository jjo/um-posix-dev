#include <stdio.h>
#include <string.h>
#include <unistd.h>

int do_rot13(char a)
{
	char b;
	return (a&64?(b=a&159)&&b<27?((b+12)%26+1)|(a&224):a:a);
}
int main(void)
{
	char buf[1024];
	int n,i;
	while ((n=read(0, buf, sizeof buf)) > 0)
	{
		for(i=0;i<n;i++)
			buf[i]=do_rot13(buf[i]);
		write(1, buf, n);
	}
	return 0;
}
