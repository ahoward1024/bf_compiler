//////// Brainfuck ////////
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

brainfuck            |      C

(Program Start)	     |      char array[infinitely large size] = {0};
					 |		char *ptr=array;
>					 |		++ptr;
<					 |		--ptr;
+					 |		++*ptr;
-					 |		--*ptr;
.					 |		putchar(*ptr);
,					 |		*ptr=getchar();
[					 |		while (*ptr) {
]					 |		}

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

char bfcommands[536870912] = {0}; //512MB
int bfcommandLength = 0;

typedef struct
{
	FILE *fptr;
	char *name;
} NFILE;

typedef struct
{
	char ptrInc; //>
	char ptrDec; //<
	char inc; //+
	char dec; //-
	char out; //.
	char in; //,
	char begin; //[
	char end; //]
} BFCMDS;

typedef struct
{
	char *ptrInc;
	char *ptrDec;
	char *inc;
	char *dec;
	char *out;
	char *in;
	char *begin;
	char *end;
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
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}

char* remapToC(CCMDS *cp, BFCMDS *bp, char c, int add)
{
	printf("Remap: c: %c\n", c);
	if(c == '\0')
	{
		return "NULL";
	}
	else if(c == (bp->ptrInc))
	{	
		char *i = convertInt(add, 10);
		char *rmap = cp->ptrInc;
		
		return cp->ptrInc;
	}
	else if (c == (bp->ptrDec))
	{
		return cp->ptrDec;
	}
	else if(c == (bp->inc))
	{
		return cp->inc;
	}
	else if(c == (bp->dec))
	{
		return cp->dec;
	}
	else if(c == (bp->out))
	{
		return cp->out;
	}
	else if(c == (bp->in))
	{
		return cp->in;
	}
	else if(c == (bp->begin))
	{
		return cp->begin;
	}
	else if(c == (bp->end))
	{
		return cp->end;
	}
	return "NULL";
}

int checkSyntax(char *fname)
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

 	if((bf.fptr = fopen(fname, "r")))
 	{
 		printf("File Opened: %s\n\n", bf.name);
 		
 		printf("File Contents:\n");
 		printf("______________\n\n");

 		while(!feof(bf.fptr)) {
 			c = fgetc(bf.fptr);
 			int check = checkChar(cmdPtr, c);
			if(check == 1)
			{
				printf("%c", c); //DEBUG
				bfcommands[symbolCount] = c;
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

 	bfcommandLength = byteCount;

 	return EXIT_SUCCESS;
}

//TODO(alex) compress
int build(char *array)
{
	CCMDS ccmds;
	CCMDS *ccmdPtr = &ccmds;
	ccmds.ptrInc = "ptr+=%;";
	ccmds.ptrDec = "ptr-=;";
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
	cfile.name = "build.c";

	int tabdepth = 1;
	int add = 1;

	if(cfile.fptr = (fopen(cfile.name, "w")))
	{
		printf("Building file : %s\n", cfile.name);
		fprintf(cfile.fptr, "// Generated with the Brainfuck C Compiler\n");
		fprintf(cfile.fptr, "#include <stdio.h>\n");
		fprintf(cfile.fptr, "int main() {\n");
		fprintf(cfile.fptr, "\tchar array[536870912] = {0}; //512MB\n");
		fprintf(cfile.fptr, "\tchar *ptr = array;\n");

		printf("bfcommandLength: %d\n", bfcommandLength);
		for(int i = 0; i < bfcommandLength; i++)
		{
			char c = bfcommands[i];
			printf("char = %c\n", c);
			//If the current character is +
			//Keep moving down the array to chain adds together
			while(c == '>')
			{
				char nextC;
				if(i < bfcommandLength-1)
				{
					add++;
					i++;
					c = bfcommands[i];
				}
			}
			c = bfcommands[i-1];

			printf("add: %d\n", add);
			char *remap;
			remap = remapToC(ccmdPtr, bfcmdsPtr, c, add);
			add = 1; //Reset the add counter
			printf("REMAP RESULT: %s\n", remap);

			if(strcmp(remap, "NULL"))
			{
				if(!strcmp(remap, ccmds.end))
				{
					tabdepth--;
				}

				for(int i = 0; i < tabdepth; i++)
				{
					fprintf(cfile.fptr, "\t");
				}

				fprintf(cfile.fptr, "%s", remap);
				fprintf(cfile.fptr, "\n");

				if(!strcmp(remap, ccmds.begin))
				{
					tabdepth++;
				}
			}
		}
		fprintf(cfile.fptr, "\treturn 0;\n");
		fprintf(cfile.fptr, "}");
	}
	else
	{
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
	if(checkSyntax("test2.bf") == EXIT_SUCCESS)
	{
		printf("\n\nBuilding %d lines of C commands.\n", bfcommandLength);
		if(build(bfcommands) == EXIT_SUCCESS)
		{
			//TODO(alex) Compress C file
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
