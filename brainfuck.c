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
 	Program Flow:
 		- Check bf file's syntax
 		- Build C file if syntax check is sucessful
 		- Compile built C file
 		- Run built C file
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char bfcommands[4096] = {0};
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

char* remapToC(CCMDS *cp, BFCMDS *bp, char c)
{
	if(c == (bp->ptrInc))
	{
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

void printErrorCaret(int count)
{
	for(int i = 0; i < count; i++)
	{
		printf(" ");
	}
	printf("^");
}

int checkSyntax(char *fname)
{
	NFILE bf;
	bf.name = fname;
	int status = 1;
	int errorMark = 0;
	char errorCharacter = '\0';
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
 	int errorCount = 0;

 	if((bf.fptr = fopen(fname, "r")))
 	{
 		printf("File Opened: %s\n\n", bf.name);

 		
 		printf("File Contents:\n");
 		printf("--------------\n\n");
 		while(!feof(bf.fptr)) {
 			c = fgetc(bf.fptr);
 			int check = checkChar(cmdPtr, c);
			if(check == 1)
			{
				printf("%c", c);
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
				errorMark = byteCount;
				errorCharacter = c;
				status = 0;
				errorCount++;
			}
			byteCount++;
 		}
 		fclose(bf.fptr);
 	}
 	else
 	{
 		if(bf.fptr == NULL)
 		{
 			printf("Error, no input files\n");
 		}
 		else
 		{
 			printf("Error, could not open file\n");
 		}
 		printf("Compiler exiting with status: failure\n");
 		return EXIT_FAILURE;
 	}

 	if(!status)
 	{
 		printf("\n");
 		printErrorCaret(errorMark);
 	}

 	printf("\n\n-------------------------\n");
 	printf("End of file.\n");
 	printf("Read bytes: %d\n", byteCount);
 	printf("Read symbols: %d\n", symbolCount);
 	printf("Skipped bytes: %d\n", skipCount);
 	printf("Error bytes: %d\n", errorCount);
 	bfcommandLength = byteCount;
 	if(status)
 	{
 		printf("Syntax check completed with status: success\n");
 	}
 	else
 	{
 		printf("Error: invalid symbol %c at character %d\n", errorCharacter, errorMark);
		printf("Syntax check completed with status: failure\n");
 	}
 	return EXIT_SUCCESS;
}

void build(char *array)
{
	printf("Building C file\n");
	CCMDS ccmds;
	CCMDS *ccmdPtr = &ccmds;
	ccmds.ptrInc = "++ptr;";
	ccmds.ptrDec = "--ptr;";
	ccmds.inc = "++*ptr;";
	ccmds.dec = "--*ptr";
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

	if(cfile.fptr = (fopen(cfile.name, "w")))
	{
		fprintf(cfile.fptr, "// Generated with the Brainfuck C Compiler\n");
		fprintf(cfile.fptr, "int main() {\n");
		fprintf(cfile.fptr, "\tchar array[4096] = {0};\n");
		fprintf(cfile.fptr, "\tchar *ptr = array;\n");
		for(int i = 0; i < bfcommandLength; i++)
		{
			char c = bfcommands[i];
			char *remap = remapToC(ccmdPtr, bfcmdsPtr, c);
			if(strcmp(remap, "NULL"))
			{
				fprintf(cfile.fptr, "\t%s", remap);
				fprintf(cfile.fptr, "\n");
			}
		}
		fprintf(cfile.fptr, "\treturn 0;\n");
		fprintf(cfile.fptr, "}");
	}
}

int compile(char * fname)
{
	char gcc[100];
	strcpy(gcc, "gcc ");
	strcat(gcc, fname);
	char *output = " -c -o c.exe";
	strcat(gcc, output);
	printf("Compiling c file: %s >> with command:  %s\n", fname, gcc);
	system(gcc);
	return 0;
}

void run(char *fname)
{
	char run[100];
	strcpy(run, "./");
	strcat(run, fname);
	system(run);
	return;
}

int main(int argc, char **argv)
{
	if(checkSyntax("helloworld.bf") == EXIT_SUCCESS)
	{
		build(bfcommands);
		compile("build.c");
		run("c.exe");
	}

 	return EXIT_SUCCESS;
}