//////// Brainfuck Compiler////////
/*  Instructions
 * > increment the data pointer to the next cell to the right
 * < decrement the data point to the next cell on the left
 * + increment the current byte by one
 * - decrement the current byte by one
 * . output the current byte
 * , accept one byte of input, store its value at the data pointer
 * [ if the byte at the data pointer is zero, instead of moving the instruction pointer forward to
 the next command, jup if forward to the command after the matching ]
 * ] if the byte at the data pointer is non zero, instead of moving the instruction pointer forward
 to the next comment jump back to the command after the matching [ 
 * 
 * The language consists of eight commands, listed below. A brainfuck program is a sequence of these
 * commands, possibly interspersed with other characters (which are ignored).
 * The commands are executed sequentially, with some exceptions: an instruction pointer begins at
 * the first command, and each command it points to is executed, after which it normally moves 
 * forward to the next command. The program terminates when the instruction pointer moves past the
 * last command.
 *
 * The brainfuck language uses a simple machine model consisting of the program and instruction
 * pointer, as well as an array of at least 30,000 byte cells initialized to zero; 
 * a movable data pointer (initialized to point to the leftmost byte of the array); and two streams 
 * of bytes for input and output (most often connected to a keyboard and a monitor respectively, 
 * and using the ASCII character encoding).
 *
 */

/*

brainfuck 			|		C

(Program Start)	    |		char array[infinitely large size] = {0};
					|		char *ptr=array;
>					|		++ptr;
<					|		--ptr;
+					|		++*ptr;
-					|		--*ptr;
.					|		putchar(*ptr);
,					|		*ptr=getchar();
[					|		while (*ptr) {
]					|		}

*/

 /*
 	Program Flow:
 		- Check bf file's syntax
 		- Build C file if syntax check is sucessful
 		- Compress C commands
 		- Compile built C file
 		- Run built C file
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <math.h>

typedef struct
{
	FILE *fptr;
	char *name;
} NFILE;

//IMPORTANT: This should really change
typedef struct
{
	char ptrInc;	// >
	char ptrDec;	// <
	char inc; 		// +
	char dec; 		// -
	char out; 		// .
	char in; 		// ,
	char begin; 	// [
	char end; 		// ]
} BFCMDS;

typedef struct
{
	char *ptrInc;	// ++ptr;
	char *ptrDec;	// --ptr;
	char *inc;		// ++*ptr;
	char *dec;		// --*ptr;
	char *out;		// putchar(*ptr);
	char *in;		// *ptr=getchar();
	char *begin;	// while(*ptr) {
	char *end;		// }
} CCMDS;

int checkChar(BFCMDS *p, char c)
{
	if(c == (p->ptrInc))
	{
		return 1;
	}
	else if (c == (p->ptrDec))
	{
		return 1;
	}
	else if(c == (p->inc))
	{
		return 1;
	}
	else if(c == (p->dec))
	{
		return 1;
	}
	else if(c == (p->out))
	{
		return 1;
	}
	else if(c == (p->in))
	{
		return 1;
	}
	else if(c == (p->begin))
	{
		return 1;
	}
	else if(c == (p->end))
	{
		return 1;
	}
	else if(c == (EOF))
	{
		return 2;
	}
	else if(c == '\n')
	{
		return 2;
	}
	else if(c == '\0')
	{
		return 2;
	}
	return 0;
}

char* convertInt(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	{
		buf[i] = "0123456789abcdef"[val % base];
	}
	
	return &buf[i+1];
	
}

int numdigits(int n)
{
	return log10(n) + 1;
}

int checkSyntax(char *fname, char *array, int *length)
{
	NFILE bf;
	bf.name = fname;

	BFCMDS bfcmds;
	BFCMDS *cmdPtr = &bfcmds;
	bfcmds.ptrInc = '>';
	bfcmds.ptrDec = '<';
	bfcmds.inc = '+';
	bfcmds.dec = '-';
	bfcmds.out = '.';
	bfcmds.in = ',';
	bfcmds.begin = '[';
	bfcmds.end = ']';

	char c = '\0';
	int byteCount = 0;
	int symbolCount = 0;
	int skipCount = 0;
	int openBraceCount = 0, closedBraceCount = 0;

	if((bf.fptr = fopen(fname, "r")))
	{
		printf("File Opened: %s\n\n", bf.name);

		printf("File Contents:\n");
		printf("______________\n\n");

		while(!feof(bf.fptr))
		{
			c = fgetc(bf.fptr);
			int check = checkChar(cmdPtr, c);
			if(check == 1)
			{
				printf("%c", c); //DEBUG
				if(c == '[')
				{
					++openBraceCount;
				}
				else if(c == ']')
				{
					++closedBraceCount;
				}

				array[symbolCount] = c;
				symbolCount++;
			}
			else if(check == 2)
			{
				skipCount++;
			}
			else
			{
				printf("%c", c);
				printf("\n\n[Error: unknown symbol: %c : at byte: %d]\n", c, byteCount);
				printf("Compiler exiting.\n");
				return EXIT_FAILURE;
			}
			byteCount++;
		}
		fclose(bf.fptr);
	}
	else
	{
		printf("\n\n[Error: could not open file %s]\n", bf.name);
		printf("Compiler exiting.\n");
		return EXIT_FAILURE;
	}

	if(closedBraceCount != openBraceCount)
	{
		printf("\n\n[Error: braces are not balanced. ");
		if(closedBraceCount > openBraceCount)
		{
			int i = closedBraceCount - openBraceCount;
			printf("Expected %d more opening brace(s).]\n", i);
		}
		//NOTE(alex) Do we actually want to ask about open braces?
		/*else if(openBraceCount > closedBraceCount)
		{
			int i = openBraceCount - closedBraceCount;
			printf("Expected %d more closing brace(s).]", i);
		}*/
		printf("Compiler exiting\n");
		return EXIT_FAILURE;
	}

	*length = byteCount;

	return EXIT_SUCCESS;
}

void remapToC(CCMDS *cp, BFCMDS *bp, char c, int number, char *rmap)
{
	if(c == (bp->ptrInc))
	{	
		char *i = convertInt(number, 10);
		strcpy(rmap, cp->ptrInc);
		strcat(rmap, i);
		strcat(rmap, ";");
		return;
	}
	else if (c == (bp->ptrDec))
	{
		char *i = convertInt(number, 10);
		strcpy(rmap, cp->ptrDec);
		strcat(rmap, i);
		strcat(rmap, ";");
		return;
	}
	else if(c == (bp->inc))
	{
		strcpy(rmap, cp->inc);
		return;
	}
	else if(c == (bp->dec))
	{
		strcpy(rmap, cp->dec);
		return;
	}
	else if(c == (bp->out))
	{
		strcpy(rmap, cp->out);
		return;
	}
	else if(c == (bp->in))
	{
		strcpy(rmap, cp->in);
		return;
	}
	else if(c == (bp->begin))
	{
		strcpy(rmap, cp->begin);
		return;
	}
	else if(c == (bp->end))
	{
		strcpy(rmap, cp->end);
		return;
	}
	rmap =  "NULL";
}

int build(char *array, int *length, char *fname)
{
	CCMDS ccmds;
	CCMDS *ccmdPtr = &ccmds;
	ccmds.ptrInc = "ptr+=";
	ccmds.ptrDec = "ptr-=";
	ccmds.inc = "++*ptr;";
	ccmds.dec = "++*ptr;";
	ccmds.out = "putchar(*ptr);";
	ccmds.in = "*ptr=getchar();";
	ccmds.begin = "while(*ptr) {";
	ccmds.end = "}";

	BFCMDS bfcmds;
	BFCMDS *bfcmdsPtr = &bfcmds;
	bfcmds.ptrInc = '>';
	bfcmds.ptrDec = '<';
	bfcmds.inc = '+';
	bfcmds.dec = '-';
	bfcmds.out = '.';
	bfcmds.in = ',';
	bfcmds.begin = '[';
	bfcmds.end = ']';

	NFILE cfile;
	cfile.name = fname;

	int tabdepth = 1; //Always starts at 1
	int number = 0; //Number to put on increments or decrements

	if(cfile.fptr = (fopen(cfile.name, "w")))
	{
		printf("Building file : %s\n", cfile.name);
		fprintf(cfile.fptr, "// Generated with the Brainfuck C Compiler\n");
		fprintf(cfile.fptr, "#include <stdio.h>\n");
		fprintf(cfile.fptr, "int main() {\n");
		fprintf(cfile.fptr, "\tchar array[%d] = {0}; //512MB\n", length);
		fprintf(cfile.fptr, "\tchar *ptr = array;\n\n");
		fprintf(cfile.fptr, "\t//Beginning of code\n\n");

		printf("bfcommandLength: %d\n", length);

		for(int i = 0; i < (*length)-1; i++)
		{
			char c = array[i];
			//If the current character is +
			//Keep moving down the array to chain adds together

			if(c == '>')
			{
				while(c == '>')
				{
					if(i < (*length))
					{
						++number;
						c = array[++i];
					}
				}
				c = array[--i];
			}
			else if(c == '<')
			{
				while(c == '<')
				{
					if(i < (*length))
					{
						++number;
						c = array[++i];
					}
				}
				c = array[--i];
			}

			char remap[100];
			remapToC(ccmdPtr, bfcmdsPtr, c, number, (char *)remap);
			number = 0; //Reset the add counter

			if(strcmp(remap, "NULL"))
			{
				if(!strcmp(remap, ccmds.end) && tabdepth != 1)
				{
					tabdepth--;
				}

				for(int i = 0; i < tabdepth; i++)
				{
					fprintf(cfile.fptr, "\t");
				}

				fprintf(cfile.fptr, "%s", remap);
				fprintf(cfile.fptr, "\n");
			}

			if(!strcmp(remap, ccmds.begin))
			{
				tabdepth++;
			}
		}
		fprintf(cfile.fptr, "\n\t//End of code\n");
		fprintf(cfile.fptr, "\n\treturn 0;\n");
		fprintf(cfile.fptr, "}");
	}
	else
	{
		printf("[Error: build failed! Could not open file\n");
		printf("Compiler exiting.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

//TODO compile
int compile()
{
	
	return EXIT_SUCCESS;
}

//TODO run
void run()
{
	
}

int main(int argc, char **argv)
{
	char *bfFileName = "helloworld.bf";
	char cbuildFileName[] = "build.c";
	char bfcommands[1048576] = {0}; //1MB
	char *bfcPtr = bfcommands;
	int bfcommandLength = 0;

	if(checkSyntax(bfFileName, bfcPtr, &bfcommandLength) == EXIT_SUCCESS)
	{
		printf("\n\nBuilding %d lines of C commands.\n", bfcommandLength);
		if(build(bfcPtr, &bfcommandLength, cbuildFileName) == EXIT_SUCCESS)
		{
			//TODO(alex) Fork to compile
			//TODO(alex) Wait until compile sucess and Run
		}
	}
	else
	{
		printf("Syntax check failed!\n");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
