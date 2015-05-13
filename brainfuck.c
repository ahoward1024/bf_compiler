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

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

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

int checkSyntax(char *fname, char **array, int *length)
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
 		return EXIT_FAILURE;
 	}

 	bfcommandLength = byteCount;
 	printf("\n\nbfcommandLength, syntaxcheck %d\n", bfcommandLength);

 	return EXIT_SUCCESS;
}

void build(char *array, int commandLength)
{
	CCMDS ccmds;
	CCMDS *ccmdPtr = &ccmds;
	ccmds.ptrInc = "++ptr;";
	ccmds.ptrDec = "--ptr;";
	ccmds.inc = "++*ptr;";
	ccmds.dec = "--*ptr;";
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
		fprintf(cfile.fptr, "#include <stdio.h>\n");
		fprintf(cfile.fptr, "int main() {\n");
		fprintf(cfile.fptr, "\tchar array[4096] = {0};\n");
		fprintf(cfile.fptr, "\tchar *ptr = array;\n");
		printf("bfcommandLength: %d\n", bfcommandLength);
		for(int i = 0; i < commandLength; i++)
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

int main(int argc, char **argv)
{
	pid_t pid;
	int processNumber = 0;
	int syntaxCheckSucessfull = 0;

	

	for (int i = 0; i < 5; ++i)
	{
    	pid = fork();
    	if (pid)
    	{
        	int status;
        	(void)waitpid(pid, &status, 0);
        	processNumber++;
    	}
    	else if (pid == 0)
    	{
    		if(processNumber == 0)
    		{
    			syntaxCheckSucessfull = checkSyntax("helloworld.bf");
    		}
    		else if(processNumber == 1)
    		{
    			if(syntaxCheckSucessfull == EXIT_SUCCESS)
    			{
    				printf("\n\nSyntax Check: sucessful\n");
    				int status;
    				printf("Building C file: build.c\n");
    				build(bfcommands);
    			}
    			else
    			{
    				printf("Syntax Check: unsuccessfull! Error in bf file\n");
    			}
    		}
    		else if(processNumber == 3)
    		{
    			int status;
    			(void)waitpid(pid, &status, 0);
    			printf("Compiling C file: gcc build.c -o build/c.exe\n");
    			system("gcc build.c -o c.exe");
    		}
    		else if(processNumber == 4)
    		{
    			int status;
    			(void)waitpid(pid, &status, 0);
    			printf("Running C file: ./c.exe\n\n");
    			system("./c.exe");
    		}
        	break;
    	}
    	else
    	{
        	printf("fork error\n");
        	exit(1);
    	}
    }


	
 	return EXIT_SUCCESS;
}