#include<stdio.h>

int strcmp(const char *s1, const char *s2)
{
	while (*s1!='\0' && *s2!='\0' && (*s1-*s2==0))
	{
		s1++;
		s2++;
	}
	return (*s1-*s2);
	
}

int main()
{
	int ret = strcmp("aav","aaa");
	printf("%d\n", ret);
	return 0;
}
