#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *result = "1";
	char *command = "ptr+=";
	char *r = result;
	char *c = command;
	
	while(*c)
	{
		printf("C: %c\n", *c);
		c++;
	}

	while(*r)
	{
		printf("R: %c\n", *r);
		r++;
	}
	c = command;
	r = result;

	*r = *c;
	
	printf("C: %c\n", *c);

	printf("result: %s\n", result);
	return 0;
}