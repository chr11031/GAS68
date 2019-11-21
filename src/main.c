#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NO_MATCH '!'
#define CHARS_PER_ENTRY 16

// Help for the Naive user
void displayOptions()
{
  char helpString[] = "No input file(s) detected. Commands are:\n" 
    "\tfile.s - input file to compile\n"
    "\t-o file.bin - generate a named output bin\n"
    "Request or write additional features to get them added to gasm68\n";

  printf(helpString);
  return;
}

// Check input ends with pattern
bool strEndsWith(char* input, char * pattern)
{
  if(!input || !pattern)
    {
      return false;
    }

  int lenInput = strlen(input);
  int lenPattern = strlen(pattern);
  if(lenPattern > lenInput)
    {
      return false;
    }

  int lenDiff = lenInput - lenPattern;
  char* ptrNearEnd = input + lenDiff;
  int match = strcmp(ptrNearEnd, pattern);
  return match == 0;
}

// Only for exact matches
bool strMatchExact(char * a, char * b)
{
  int idx = strcmp(a, b);
  return idx == 0;
}

// Compiler Option
typedef struct
{
  char match[CHARS_PER_ENTRY];
  bool (*matchFun)(char*, char*);
  char flag;
} CompilerOption;

// Figure out which CLI options were used
char determineCase(char *input)
{
  // List of options
  CompilerOption options[] = {
    {.match = ".s",    .matchFun = &strEndsWith,   .flag = 'f'},
    {.match = "-o",    .matchFun = &strMatchExact, .flag = 'o'},
    {.match = "-help", .matchFun = &strMatchExact, .flag = 'h'}
  };

  // Check for matches
  int numOptions = sizeof(options) / sizeof(CompilerOption);
  int i;
  for(i = 0; i < numOptions; i++)
    {
      if(options[i].matchFun(input, options[i].match))
	{
	  return options[i].flag;
	}
    }

  // Speciial pattern
  return NO_MATCH;
}

// Entry Point
int main(int argc, char **argv)
{
  // At least one input required
  if(argc == 0)
    {
      displayOptions();
      return 0;
    }

  // Process CLI directives
  char* inputFile = NULL;
  char* outFile = "a.bin";
  int arg;
  for(arg = 1; arg < argc; arg++)
    {
      char flag = determineCase(argv[arg]);
      switch(flag)
	{
	case 'h':
	  {
	    displayOptions();
	    goto ERROR;
	  }
	case 'f':
	  {
	      {
		inputFile = argv[arg];
	      }
	  }
	  break;
	case 'o':
	  {
	    if(arg+1 > argc-1)
	      {
		printf("Missing file parameter\n");
		goto ERROR;
	      }
	    else
	      {
		outFile = argv[++arg];
	      }
	  }
	  break;
	case NO_MATCH:
	  printf("Unkown flag: %s ABORTING\n", argv[arg]);
	default:
	ERROR:
	  printf("Error: aborting\n");
	  exit(1);
	  break;
	}
    }
  printf("Input file is %s --- Output file is %s\n", inputFile, outFile);

  return 0;
}
