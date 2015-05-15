// Generated with the Brainfuck C Compiler
#include <stdio.h>
int main() {
	char array[536870912] = {0}; //512MB
	char *ptr = array;

	//Beginning of code

	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	while(*ptr) {
		ptr+=1;
		++*ptr;
		++*ptr;
		++*ptr;
		++*ptr;
		while(*ptr) {
			ptr+=1;
			++*ptr;
			++*ptr;
			ptr+=1;
			++*ptr;
			++*ptr;
			++*ptr;
			ptr+=1;
			++*ptr;
			++*ptr;
			++*ptr;
			ptr+=1;
			++*ptr;
			ptr-=4;
			++*ptr;
		}
		ptr+=1;
		++*ptr;
		ptr+=1;
		++*ptr;
		ptr+=1;
		++*ptr;
		ptr+=2;
		++*ptr;
		while(*ptr) {
			ptr-=1;
		}
		ptr-=1;
		++*ptr;
	}
	ptr+=2;
	putchar(*ptr);
	ptr+=1;
	++*ptr;
	++*ptr;
	++*ptr;
	putchar(*ptr);
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	putchar(*ptr);
	putchar(*ptr);
	++*ptr;
	++*ptr;
	++*ptr;
	putchar(*ptr);
	ptr+=2;
	putchar(*ptr);
	ptr-=1;
	++*ptr;
	putchar(*ptr);
	ptr-=1;
	putchar(*ptr);
	++*ptr;
	++*ptr;
	++*ptr;
	putchar(*ptr);
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	putchar(*ptr);
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	++*ptr;
	putchar(*ptr);
	ptr+=2;
	++*ptr;
	putchar(*ptr);
	ptr+=1;
	++*ptr;
	++*ptr;
	putchar(*ptr);

	//End of code

	return 0;
}