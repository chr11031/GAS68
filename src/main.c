#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INVALID_FLAG '!'
#define MAX_FLAG_LEN 16
#define MAX_LINE_SIZE 1024
#define MAX_BYTES_PER_OPCODE 8

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

// Create a bin file at specified destination
bool writeBin(char* outFile, char* machineCode, int len)
{
  FILE* fp;
  fp = fopen(outFile, "wb");
  if(fp == NULL) return false;
  char * end = machineCode + len;
  fwrite(&machineCode, sizeof(char), len, fp);
  fclose(fp);
  return true;
}

// Read source file, convert to machine code, return length
bool parseSource(char* inFile, char** machineCode, int* machineLen)
{
  // Open file, count newlines, rewind
  FILE* fp;
  fp = fopen(inFile, "r");
  if(fp == NULL) return false;
  fseek(fp, 0, SEEK_END);
  int numLines = 0;
  while(!feof(fp))
    {
      char symbol = fgetc(fp);
      if(symbol == '\n')
	{
	  numLines += 1;
	}
    }
  unsigned long len = (unsigned long)ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // Create 2D file array
  char** lineByLine = (char**)malloc(sizeof(char*) * numLines);
  int i;
  for(i = 0; i < numLines; i++)
    {
      lineByLine[i] = (char*)malloc(sizeof(char) * MAX_LINE_SIZE);
      fgets(lineByLine[i], MAX_LINE_SIZE, fp);
   }
  fclose(fp);

  // Create machine code buffer
  *machineCode = (char*)malloc(sizeof(char) * numLines * MAX_BYTES_PER_OPCODE);
  char* machinePtr = *machineCode;

  // Test return statement
  char results[] = "BlahBlah\n";
  *machineCode = results;
  *machineLen = 9;
  return true;
}

// Check input ends with pattern
bool strEndsWith(char* input, char * pattern)
{
  // Check NULL inputs
  if(!input || !pattern)
    {
      return false;
    }

  // Find length of strings
  int lenInput = strlen(input);
  int lenPattern = strlen(pattern);
  if(lenPattern > lenInput)
    {
      return false;
    }

  // Compare end with pattern
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
  char match[MAX_FLAG_LEN];
  bool (*matchFun)(char*, char*);
  char flag;
} CompilerOption;

// Figure out which CLI options were used
char determineFlag(char *input)
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
  return INVALID_FLAG;
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
      char flag = determineFlag(argv[arg]);
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
	case INVALID_FLAG:
	  printf("Unkown flag: %s ABORTING\n", argv[arg]);
	default:
	ERROR:
	  printf("Error: aborting\n");
	  exit(1);
	  break;
	}
    }

  // Ensure an input was given
  if(inputFile == NULL)
    {
      printf("No input file specified. Aborting\n");
      return 1;
    }
  printf("====GAS68_MOTOROLA_68000_ASSEMBLER====\nInput file: %s\nOutput file: %s\n", inputFile, outFile);

  // Read file + Convert
  int machineLen = 0;
  char* machineCode;
  if(!parseSource(inputFile, &machineCode, &machineLen))
    {
      printf("Error parsing file: %s\n", inputFile);
      return 1;
    }

  // Output to binary file
  if(!writeBin(outFile, machineCode, machineLen))
    {
      printf("Could not write to %s\n", outFile);
      return 1;
    }

  // Report completion
  printf("Asembling done.\n");
  return 0;
}
