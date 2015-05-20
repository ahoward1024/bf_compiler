// Generated with the Brainfuck C Compiler
#include <stdio.h>
int main(int argc, char **argv) {
	char array[107] = {0}; //1MB maximum (1048576 bytes)
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