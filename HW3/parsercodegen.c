/*
  Aouthors' names:

  Faramarz Aboutalebi
  Ethan Stein

*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARRAY_LENGTH 1000
// Maximum length of the number
#define MAX_DIGITS_LENGTH 5
// Maximum length of the
#define MAX_CHARACTER_LENGTH 11
// Maximum size of the symbol table
#define MAX_SYMBOL_TABLE_SIZE 500

// file pointer
FILE *fp2;

typedef enum
{
  skipsym = 1,
  identsym = 2,
  numbersym = 3,
  plussym = 4,
  minussym = 5,
  multsym = 6,
  slashsym = 7,
  ifelsym = 8,
  eqlsym = 9,
  neqsym = 10,
  lessym = 11,
  leqsym = 12,
  gtrsym = 13,
  geqsym = 14,
  lparentsym = 15,
  rparentsym = 16,
  commasym = 17,
  semicolonsym = 18,
  periodsym = 19,
  becomessym = 20,
  beginsym = 21,
  endsym = 22,
  ifsym = 23,
  thensym = 24,
  whilesym = 25,
  dosym = 26,
  callsym = 27,
  constsym = 28,
  varsym = 29,
  procsym = 30,
  writesym = 31,
  readsym = 32,
  elsesym = 33
} token_type;

// struture to store the lexeme and token
typedef struct
{
  char string[MAX_ARRAY_LENGTH];
  int Token;
} subString;

// struct to save the idenetifiers
typedef struct
{
  char id[MAX_CHARACTER_LENGTH];
} stringHolder;

// data structure for the symbol.
typedef struct
{
  int kind;      // const = 1, var = 2, proc = 3
  char name[10]; // name up to 11 chars
  int val;       // number (ASCII value)
  int level;     // L level
  int addr;      // M address
  int mark;      // to indicate unavailable or deleted
} symbol;

// Global Symbol table
symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
int sizeOfSymbolTable = 0;

// This function takes the input anf convert it to subString and store them to the subString array which is a sttructure array
int subStringCreater(char *inputArr, int sizeOfinputArr, subString *subString, int sizeOfsubString, int stringIndex)
{ // function

  for (int i = 0; i < sizeOfinputArr; i++)
  {

    if (inputArr[i] >= 0 && inputArr[i] <= 32) // ASCII of invisivle charactors
    {
      continue;
    }
    if (isdigit(inputArr[i]))
    { // if the charactor is a digit

      stringIndex = 0;
      // add the digit to the string
      subString[sizeOfsubString].string[stringIndex++] = inputArr[i];

      // while the next charactors are digits add them to the string
      while (isdigit(inputArr[i + 1]))
      {
        i++;
        subString[sizeOfsubString].string[stringIndex++] = inputArr[i];
      }
      // add null terminator
      subString[sizeOfsubString].string[stringIndex] = '\0';

      // set stringIndex to 0 to use it for creating another string
      stringIndex = 0;
      // increment sizeOfsubString to create another string
      sizeOfsubString++;
    }
    else if (isalpha(inputArr[i]))
    { // identifier

      stringIndex = 0;

      // add the alphabet to the string
      subString[sizeOfsubString].string[stringIndex++] = inputArr[i];

      // while the next charactors are alphabet or digits add them to the string
      while (isalpha(inputArr[i + 1]) || isdigit(inputArr[i + 1]))
      {
        i++;
        subString[sizeOfsubString].string[stringIndex++] = inputArr[i];
      }

      // add null terminator
      subString[sizeOfsubString].string[stringIndex] = '\0';

      // set stringIndex to 0 to use it for creating another string
      stringIndex = 0;
      // increment sizeOfsubString to create another string
      sizeOfsubString++;
    }
    else if (((inputArr[i] == '<' && inputArr[i + 1] == '>') || (inputArr[i] == '<' && inputArr[i + 1] == '=') || (inputArr[i] == '>' && inputArr[i + 1] == '=') || (inputArr[i] == ':' && inputArr[i + 1] == '=')))
    { // take care of <>   <=   >=   :=

      stringIndex = 0;

      // add < or > or : to the string
      subString[sizeOfsubString].string[stringIndex++] = inputArr[i];
      // add > or = to the string
      subString[sizeOfsubString].string[stringIndex++] = inputArr[i + 1];

      // add null terminator
      subString[sizeOfsubString].string[stringIndex] = '\0';

      // update the index to last used element
      i = i + 1;

      // set stringIndex to 0 to use it for creating another string
      stringIndex = 0;
      // increment sizeOfsubString to create another string
      sizeOfsubString++;
    }
    else if ((inputArr[i] == '/' && inputArr[i + 1] == '*'))
    { // handle comment

      stringIndex = 0;

      // Traversing in the array to see if */ exsists
      int j;
      for (j = i + 1 + 1; j < sizeOfinputArr; j++)
      {
        if (inputArr[j] == '*' && inputArr[j + 1] == '/')
        {
          // update the index to skip the comment
          i = j + 1;
          break;
        }
      }
      if (i < j + 1)
      { //  if */ did not find

        // creates two diffrent subStrings, one for the star and one for the slash
        subString[sizeOfsubString].string[stringIndex] = inputArr[i];
        stringIndex++;
        // add null terminator
        subString[sizeOfsubString].string[stringIndex] = '\0';
        sizeOfsubString++;
        // next string
        stringIndex = 0;
        subString[sizeOfsubString].string[stringIndex] = inputArr[i + 1];
        stringIndex++;
        // add null terminator
        subString[sizeOfsubString].string[stringIndex] = '\0';
        sizeOfsubString++;

        // update the index to last used element
        i = i + 1;
      }
    }
    else if (inputArr[i] != ' ' && inputArr[i] != '\t' && inputArr[i] != '\n')
    { // taking care of sybmols and oprators

      subString[sizeOfsubString].string[stringIndex++] = inputArr[i];
      // add null terminator
      subString[sizeOfsubString].string[stringIndex] = '\0';

      // set stringIndex to 0 to use it for creating another string
      stringIndex = 0;
      // increment sizeOfsubString to create another string
      sizeOfsubString++;
    }
  }

  return sizeOfsubString;

} // end

/******************************************************/
int isThisAnumber(char string[])
{ // checks if the string is a number

  int sizeOfstring = strlen(string);

  for (int i = 0; i < sizeOfstring; i++)
  {
    if (!isdigit(string[i]))
      return 0;
  }
  return 1;
}

/******************************************************/

void tokenCreator(subString *subString, int sizeOfsubString)
{ // function

  for (int i = 0; i < sizeOfsubString; i++)
  {

    // plus
    if (strcmp(subString[i].string, "+") == 0)
    {
      subString[i].Token = plussym;
    }
    // minus
    else if (strcmp(subString[i].string, "-") == 0)
    {
      subString[i].Token = minussym;
    }
    // multiplication
    else if (strcmp(subString[i].string, "*") == 0)
    {
      subString[i].Token = multsym;
    }
    // slash
    else if (strcmp(subString[i].string, "/") == 0)
    {
      subString[i].Token = slashsym;
    }
    // ifel
    else if (strcmp(subString[i].string, "ifel") == 0)
    {
      subString[i].Token = ifelsym;
    }
    // equal
    else if (strcmp(subString[i].string, "=") == 0)
    {
      subString[i].Token = eqlsym;
    }
    // non equal
    else if (strcmp(subString[i].string, "<>") == 0)
    {
      subString[i].Token = neqsym;
    }
    // less than
    else if (strcmp(subString[i].string, "<") == 0)
    {
      subString[i].Token = lessym;
    }
    // less than or equal to
    else if (strcmp(subString[i].string, "<=") == 0)
    {
      subString[i].Token = leqsym;
    }
    // greater than
    else if (strcmp(subString[i].string, ">") == 0)
    {
      subString[i].Token = gtrsym;
    }
    // greater than or equal to
    else if (strcmp(subString[i].string, ">=") == 0)
    {
      subString[i].Token = geqsym;
    }
    // left parenthesis
    else if (strcmp(subString[i].string, "(") == 0)
    {
      subString[i].Token = lparentsym;
    }
    // right parenthesis
    else if (strcmp(subString[i].string, ")") == 0)
    {
      subString[i].Token = rparentsym;
    }
    // comma
    else if (strcmp(subString[i].string, ",") == 0)
    {
      subString[i].Token = commasym;
    }
    // semicolon
    else if (strcmp(subString[i].string, ";") == 0)
    {
      subString[i].Token = semicolonsym;
    }
    // period
    else if (strcmp(subString[i].string, ".") == 0)
    {
      subString[i].Token = periodsym;
    }
    // become
    else if (strcmp(subString[i].string, ":=") == 0)
    {
      subString[i].Token = becomessym;
    }
    // begin
    else if (strcmp(subString[i].string, "begin") == 0)
    {
      subString[i].Token = beginsym;
    }
    // end
    else if (strcmp(subString[i].string, "end") == 0)
    {
      subString[i].Token = endsym;
    }
    // if
    else if (strcmp(subString[i].string, "if") == 0)
    {
      subString[i].Token = ifsym;
    }
    // then
    else if (strcmp(subString[i].string, "then") == 0)
    {
      subString[i].Token = thensym;
    }
    // while
    else if (strcmp(subString[i].string, "while") == 0)
    {
      subString[i].Token = whilesym;
    }
    // do
    else if (strcmp(subString[i].string, "do") == 0)
    {
      subString[i].Token = dosym;
    }
    // call
    else if (strcmp(subString[i].string, "call") == 0)
    {
      // subString[i].Token = callsym;
      //  In HW3 call is an identifire
      subString[i].Token = identsym;
    }
    // const
    else if (strcmp(subString[i].string, "const") == 0)
    {
      subString[i].Token = constsym;
    }
    // var
    else if (strcmp(subString[i].string, "var") == 0)
    {
      subString[i].Token = varsym;
    }
    // procedure
    else if (strcmp(subString[i].string, "procedure") == 0)
    {
      // subString[i].Token = procsym;
      // In HW3 call is an identifire
      subString[i].Token = identsym;
    }
    // write
    else if (strcmp(subString[i].string, "write") == 0)
    {
      subString[i].Token = writesym;
    }
    // read
    else if (strcmp(subString[i].string, "read") == 0)
    {
      subString[i].Token = readsym;
    }
    // else
    else if (strcmp(subString[i].string, "else") == 0)
    {
      subString[i].Token = elsesym;
    }
    // number
    else if (isThisAnumber(subString[i].string))
    {

      if (strlen(subString[i].string) <= MAX_DIGITS_LENGTH)
        subString[i].Token = numbersym;
      else
        subString[i].Token = -1; // Error : Numbers cannot exceed 5 digits
    }
    // idenfier
    else if (isalpha(subString[i].string[0]) || isdigit(subString[i].string[0]))
    {
      if (strlen(subString[i].string) > MAX_CHARACTER_LENGTH)
        subString[i].Token = -2; // Error : Identifiers cannot exceed 11 characters in length
      else
        subString[i].Token = identsym;
    }

    else
      subString[i].Token = -3; // Error : Invalid Symbol
  }
}

/******************************************************/

// prints out the lexeme table
void LexemeTable(subString *subString, int sizeOfsubString)
{

  printf("Lexeme Table:\n");
  fprintf(fp2, "Lexeme Table:\n"); // print to output file

  printf("lexeme\t\ttoken type\n");
  fprintf(fp2, "lexeme      token type\n"); // print to output file

  // prints out the lexemes
  for (int i = 0; i < sizeOfsubString; i++)
  {

    if (subString[i].Token > 0)
    { // check if token is valid

      printf("%s", subString[i].string);
      fprintf(fp2, "%s", subString[i].string); // print to output file

      // creat the space to build the lexeme table
      int len = strlen(subString[i].string);
      for (int j = 0; j < 12 - len; j++)
      {
        printf(" ");
        fprintf(fp2, " "); // print to output file
      }
    }
    // prints the erorrs
    if (subString[i].Token == -1)
    {
      printf("Error : Number too long.\n");
      fprintf(fp2, "Error : Number too long.\n"); // print to output file
    }
    else if (subString[i].Token == -2)
    {
      printf("Error : Name too long.\n");
      fprintf(fp2, "Error : Name too long.\n"); // print to
    }
    else if (subString[i].Token == -3)
    {
      printf("Error : Invalid Symbols.\n");
      fprintf(fp2, "Error : Invalid Symbols.\n"); // print to output file
    }
    else
    {
      printf("%d\n", subString[i].Token);
      fprintf(fp2, "%d\n", subString[i].Token); // print to output file
    }
  }
}

/******************************************************/

// convering string to number
int str_to_int(const char *str)
{
  int result = 0;
  int sign = 1; // To handle negative numbers

  // Check for a negative sign
  if (*str == '-')
  {
    sign = -1;
    str++; // Move to the next character
  }

  while (*str)
  {
    // Ensure the character is a digit
    if (*str < '0' || *str > '9')
    {
      printf("Invalid character detected: %c\n", *str);
      return 0; // Or handle the error as required
    }

    result = (result * 10) + (*str - '0');
    str++;
  }

  return result * sign;
}

/******************************************************/

// prints out the token list
void TokenListAndTokenArrayPopulat(subString *subString, int sizeOfsubString, int token_array[], int *sizeOftoken_arra, stringHolder identArray[], int *sizeOfidentArray)
{

  printf("Token List:\n");
  fprintf(fp2, "Token List:\n"); // print to output file

  for (int i = 0; i < sizeOfsubString; i++)
  {
    // prints valid tokens
    if (subString[i].Token > 0)
    {
      token_array[(*sizeOftoken_arra)++] = subString[i].Token; //...........

      printf("%d ", subString[i].Token);
      fprintf(fp2, "%d ", subString[i].Token); // print to output file
    }
    if (subString[i].Token == 2 || subString[i].Token == 3)
    {
      if (subString[i].Token == 3)
        token_array[(*sizeOftoken_arra)++] = str_to_int(subString[i].string);
      else
      {

        strcpy(identArray[(*sizeOfidentArray)].id, subString[i].string);
        token_array[(*sizeOftoken_arra)++] = *sizeOfidentArray;
        (*sizeOfidentArray)++;
      }

      printf("%s ", subString[i].string);
      fprintf(fp2, "%s ", subString[i].string); // print to output file
    }
  }
  printf("\n");
  fprintf(fp2, "\n"); // print to output file
}

/******************************************************/

// implement symbol table

int symbolTableCheck(stringHolder name)
{
  // Linear search through symbol table looking at name. Returns index if found, -1 otherwise.
  int i;
  for (i = 0; i < sizeOfSymbolTable; i++)
    if (strcmp(symbolTable[i].name, name.id) == 0)
      return i;
  return -1;
}

void addToSymbolTable(int kind, stringHolder name, int value, int addr, int mark)
{
  printf("Adding (kind, name, value): %d %s %d\n", kind, name.id, value);
  symbol s;
  s.kind = kind;
  strcpy(s.name, name.id);
  s.val = value;
  s.addr = addr;
  s.mark = mark;

  symbolTable[sizeOfSymbolTable] = s;
  sizeOfSymbolTable++;
}

void SymbolTable(stringHolder identArray[], int sizeOfidentArray, int token_array[], int sizeOftoken_array)
{
  // const = 1, var = 2, proc = 3
  // for HW3 we only have const and var
  // looking for 'const' and 'var' in the toke array
  // int address = 3;
  // int ident_index = 0;
  // for(int i = 0; i < sizeOftoken_arra; i++){

  //   //For constants, you must store kind, name and value.
  //   // if the token is a const. making sure 28 represnts const not number
  //   if(token_array[i] == 28 ){

  //     i+=5;// to get acsess to the value of const in the token_array
  //     symbolTable[*sizeOfSymbolTable].kind = 1;
  //     strcpy(symbolTable[*sizeOfSymbolTable].name, identArray[ident_index++].id);
  //     symbolTable[*sizeOfSymbolTable].mark = 1;
  //     symbolTable[*sizeOfSymbolTable].val = token_array[i];
  //     i++;//updat the index for the next checking
  //     (*sizeOfSymbolTable)++;

  //   }
  //   //if the token is a var. making sure 29 represnts var not number
  //   else if((i == 0 && token_array[0] == 29) ||(token_array[i] == 29 && i >0 && token_array[i-1] != 3)){

  //     do{
  //       i++;//// to get acsess to the name of the ident

  //       symbolTable[*sizeOfSymbolTable].kind = 2;
  //       strcpy(symbolTable[*sizeOfSymbolTable].name, identArray[ident_index++].id);
  //       symbolTable[*sizeOfSymbolTable].level = 0;
  //       symbolTable[*sizeOfSymbolTable].addr = address++;
  //       symbolTable[*sizeOfSymbolTable].mark = 1;
  //       (*sizeOfSymbolTable)++;
  //       i+=2;
  //     }while(token_array[i] == 17);//if the token is a comma means ther is more idents

  //   }
  // }

  int constant = 1, var = 2, proc = 3;

  sizeOfSymbolTable = 0;

  int i = 0, j = 0, t;

  stringHolder temp;

  while (i < sizeOfidentArray) // Loop through token array
  {
    t = token_array[i];

    // Constant Declaration
    if (t == constsym)
    {
      do
      {
        // Get next token
        t = token_array[++i];
        if (t != identsym)
          printf("Error invalid identity\n");
        if (symbolTableCheck(identArray[j]) != -1)
          printf("Error token already exists\n");
        // Save identity name
        temp = identArray[j++];
        i++;
        // Get next token
        t = token_array[++i];
        if (t != eqlsym)
          printf("Error missing equals following name in declaration\n");
        // Get next token
        t = token_array[++i];
        if (t != numbersym)
          printf("Error name not assigned to proper numerical value\n");
        // Add to symbol table
        addToSymbolTable(1, temp, token_array[++i], 0, 0);
      } while (t == commasym);
    }

    i++; // Increment token array index
  }
  // Var Declaration
  // Statement
  // Condition
  // Expression
  // Term
  // ---Factor
  // Factor
}

/******************************************************/
void printSymbolTable()
{

  printf("Kind | Name        | Value | Level | Address | Mark\n");
  printf("---------------------------------------------------\n");

  for (int i = 0; i < sizeOfSymbolTable; i++)
  {

    printf("%d |", symbolTable[i].kind);

    for (int j = 0; j < 12 - strlen(symbolTable[i].name); j++)
    {
      printf(" ");
    }

    printf("%s |     %d |", symbolTable[i].name, symbolTable[i].val);
    if (symbolTable[i].kind == 2)
      printf("     %d |     %d |", symbolTable[i].level, symbolTable[i].addr);
    else
      printf("     - |     - |");
    printf("     %d\n", symbolTable[i].mark);
  }
}

/******************************************************/

int main(int argc, char *argv[])
{

  if (argc < 2)
  { // checks if there is a file
    printf("Error : please include the file name");
    return 1;
  }

  FILE *fp = fopen(argv[1], "r"); // opens the file

  if (fp == NULL)
  { // checks if the file is valid
    printf("Error : cannot open file");
    return 1;
  }

  // define the inputArr array and subString array
  char inputArr[MAX_ARRAY_LENGTH];
  subString subString[MAX_ARRAY_LENGTH];
  // define a variable to track the size of the inputArr array
  int sizeOfinputArr = 0;
  // define a variable to track the size of the subString array
  int sizeOfsubString = 0;

  char c;
  // read character input by fscanf
  while (fscanf(fp, "%c", &c) != EOF)
  {
    // reads the file and store chacracter by chacracter in the arr
    inputArr[sizeOfinputArr] = c;
    sizeOfinputArr++;
  }

  // close fp
  fclose(fp);

  // set the end of the inputArr to null
  inputArr[sizeOfinputArr] = '\0';

  // open a file in w mode
  fp2 = fopen("output.txt", "w");

  // print the source code
  printf("Source Program:\n");
  fprintf(fp2, "Source Program:\n"); // print to file
  for (int i = 0; i < sizeOfinputArr; i++)
  {
    printf("%c", inputArr[i]);
    fprintf(fp2, "%c", inputArr[i]); // print to file
  }

  // arrTracker
  // arrTracker is the last index of the array
  int stringIndex = 0;

  // call function to creat subStrings and return the # of the strings
  sizeOfsubString = subStringCreater(inputArr, sizeOfinputArr, subString, sizeOfsubString, stringIndex); // call function

  // call function to generate token for eac subString
  tokenCreator(subString, sizeOfsubString); // call function

  printf("\n\n");
  fprintf(fp2, "\n\n"); // print to file

  // call function to print the lexeme table
  LexemeTable(subString, sizeOfsubString);

  // save the tokens in an aray
  int token_array[MAX_ARRAY_LENGTH];
  int sizeOftoken_array = 0;
  // save the idenfier in an array
  stringHolder identArray[MAX_ARRAY_LENGTH];
  int sizeOfidentArray = 0;

  // call function to print the token list, and populating the token_array
  TokenListAndTokenArrayPopulat(subString, sizeOfsubString, token_array, &sizeOftoken_array, identArray, &sizeOfidentArray);

  // print the token array
  printf("Token Array:\n");
  for (int i = 0; i < sizeOftoken_array; i++)
  {
    printf("%d ", token_array[i]);
  }

  printf("\n");

  // implemnting symbol table
  SymbolTable(identArray, sizeOfidentArray, token_array, sizeOftoken_array);

  printf("\n\n");

  printSymbolTable();

  // close fp2
  fclose(fp2);

  return 0;
}
