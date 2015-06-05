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

Brainfuck  		|		C

(Program Start)	|		char array[infinitely large size] = {0};
				|		char *ptr=array;
>				|		++ptr;
<				|		--ptr;
+				|		++*ptr;
-				|		--*ptr;
.				|		putchar(*ptr);
,				|		*ptr=getchar();
[				|		while (*ptr) {
]				|		}

*/

 /*
 	Program Flow:
 		- Check bf file's syntax
 		- Build C file if syntax check is sucessful
 		- Compress C commands
 		- Compile built C file
 		- Run built C file
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <windows.h>

#ifdef _WIN32
#define open(p, f, m) _open(p, f, m)
#endif

typedef struct
{
	FILE *fptr;
 	char *name;
} NFILE;

int checkChar(char c)
{
	switch(c)
	{
		case '>'  : return 1;
		case '<'  : return 1;
		case '+'  : return 1;
		case '-'  : return 1;
		case '.'  : return 1;
		case ','  : return 1;
		case '['  : return 1;
		case ']'  : return 1;
		case EOF  : return 2;
		case '\n' : return 2;
		case '\0' : return 2;
		default   : return 0;
	}
}

int checkSyntax(char *fname, char *array, int *length)
{
	NFILE brainfuckFile;
	brainfuckFile.name = fname;

	char c = '\0';
	int byteCount = 0;
	int symbolCount = 0;
	int skipCount = 0;
	int openBraceCount = 0, closedBraceCount = 0;

	if((brainfuckFile.fptr = fopen(brainfuckFile.name, "r")))
	{
		printf("File Opened: %s\n\n", brainfuckFile.name);

		printf("File Contents:\n");
		printf("______________\n\n");

		while(!feof(brainfuckFile.fptr))
		{
			c = fgetc(brainfuckFile.fptr);
			int check = checkChar(c);
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
				++symbolCount;
			}
			else if(check == 2)
			{
				++skipCount;
			}
			else
			{
				printf("%c", c);
				printf("\n\n[Error: unknown symbol: %c : at byte: %d]\n", c, byteCount);
				printf("Compiler exiting.\n");
				return EXIT_FAILURE;
			}
			++byteCount;
		}
		fclose(brainfuckFile.fptr);
	}
	else
	{
		printf("\n\n[Error: could not open file %s]\n", brainfuckFile.name);
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
		printf("Compiler exiting\n");
		return EXIT_FAILURE;
	}

	*length = byteCount;

	return EXIT_SUCCESS;
}

void remapToC(char c, char *rmap)
{
	switch(c)
	{
		case '>' : strcpy(rmap, "++ptr;"); 			return;
		case '<' : strcpy(rmap, "--ptr;"); 			return;
		case '+' : strcpy(rmap, "++*ptr;");			return;
		case '-' : strcpy(rmap, "--*ptr;");			return;
		case '.' : strcpy(rmap, "putchar(*ptr);");	return;
		case ',' : strcpy(rmap, "*ptr=getchar();"); return;
		case '[' : strcpy(rmap, "while(*ptr) {");	return;
		case ']' : strcpy(rmap, "}");				return;
		default  : strcpy(rmap, "NULL");
	}
}

int build(char *array, int length, char *fname)
{
	NFILE cfile;
	cfile.name = fname;

	int tabdepth = 1; //Always starts at 1
	int number = 0; //Number to put on increments or decrements

	if(cfile.fptr = (fopen(cfile.name, "w")))
	{
		printf("Building file : %s\n", cfile.name);
		fprintf(cfile.fptr, "// Generated with the Brainfuck C Compiler\n");
		fprintf(cfile.fptr, "#include <stdio.h>\n");
		fprintf(cfile.fptr, "int main(int argc, char **argv) {\n");
		fprintf(cfile.fptr, "\tchar array[%d] = {0}; //1MB maximum (1048576 bytes)\n", length);
		fprintf(cfile.fptr, "\tchar *ptr = array;\n\n");
		fprintf(cfile.fptr, "\t//Beginning of code\n\n");

		//printf("DEBUG: fcommandLength: %d\n", length);

		for(int i = 0; i < length-1; i++)
		{
			char c = array[i];
			char remap[16];
			remapToC(c, (char *)remap);
			number = 0; //Reset the add counter

			if(strcmp(remap, "NULL"))
			{
				if(!strcmp(remap, "}") && tabdepth != 1)
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

			if(!strcmp(remap, "while(*ptr) {"))
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
	fclose(cfile.fptr);
	return EXIT_SUCCESS;
}

int compile()
{
	printf("Running gcc with command : gcc -g -Wall build.c -o build.exe\n");

	FILE *compileOutput;
	compileOutput = popen("gcc -g -Wall build.c -o build.exe 2>&1", "r");
	char buffer[1024];

	if(!compileOutput)
	{
		fprintf(stderr, "Running compiler failed! Fatal error. Exiting.\n");
		return EXIT_FAILURE;
	}
	else
	{
		printf("Compilation completed. Status: ");
		fgets(buffer, 256, compileOutput);
		if(buffer[0] == '\0')
		{
			printf("sucessful\n\n");
		}
		else
		{
			printf("ERROR :\n\n");
			puts(buffer);
			return EXIT_FAILURE;
		}
	}

	if(pclose(compileOutput) != 0)
	{
		fprintf(stderr, "Could not close pipe. Fatal error. Exiting\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	char *bfFileName = "helloworld.bf";
	char cbuildFileName[] = "build.c";
	char bfcommands[1048576] = {0}; //1MB
	int bfcommandLength = 0;
	int *buildStatus = 0;

	if(checkSyntax(bfFileName, (char *)bfcommands, &bfcommandLength) == EXIT_SUCCESS)
	{
		printf("\n\nBuilding %d lines of C commands.\n", bfcommandLength);
		if(build((char *)bfcommands, bfcommandLength, cbuildFileName) == EXIT_SUCCESS)
		{
			NFILE cBuildFile;
			cBuildFile.name = cbuildFileName;
			if(cBuildFile.fptr = fopen(cBuildFile.name, "r"))
			{
				if(!compile() == EXIT_SUCCESS)
				{
					return EXIT_FAILURE;
				}
			}
			else
			{
				printf("Error: could not open file %s for compilation\n. Exiting.\n", cBuildFile.name);
			}
		}
	}
	else
	{
		printf("Syntax check failed!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
