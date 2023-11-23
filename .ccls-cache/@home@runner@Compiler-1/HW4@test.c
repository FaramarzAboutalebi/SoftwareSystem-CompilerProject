/*
  Aouthors' names:

  Faramarz Aboutalebi
  Ethan Stein

*/
//.
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARRAY_LENGTH 1000
// Maximum length of the number
#define MAX_DIGITS_LENGTH 5
// Maximum length of the
#define MAX_CHARACTER_LENGTH 11
// Maximum size of the symbol table
#define MAX_SYMBOL_TABLE_SIZE 500

//  data structure for the symbol.
typedef struct {
  int kind;      // const = 1, var = 2, proc = 3
  char name[12]; // name up to 11 chars
  int val;       // number (ASCII value)
  int level;     // L level
  int addr;      // M address
  int mark;      // to indicate unavailable or deleted
} symbol;

// struct to save the idenetifiers
typedef struct {
  char id[MAX_ARRAY_LENGTH];
} stringHolder;

// save the tokens in an aray
int token_array[MAX_ARRAY_LENGTH];
int sizeOftoken_arra = 0; // size of the array of tokens

// save the idenfier in an array
stringHolder identArray[MAX_ARRAY_LENGTH];
int sizeOfidentArray = 0; // size of the array of identifiers

int seek; // seek is an index of the symbolTable

// symbol table array and its size
symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
int sizeOfSymbolTable = 0;

int currentToken = -1; // index of the current token to traverse in token_array
int TOKEN;             // token holder

typedef struct {
  int destination;
  char name[12];
} calInfo;

calInfo callArray[MAX_ARRAY_LENGTH];
int sizeOfCallArray = 0;

// instrution struct
typedef struct {
  int op;
  int L;
  int M;
} instruction;

// instruction array
instruction text[MAX_ARRAY_LENGTH];

int cx = 0;           // size of the text array
int CODE_SIZE = 1000; // max size of the text array
// ############################

int level = -1; // lexigraphical level

// file pointer
FILE *fp2;

// adding $8 to the to hide them callsym and procsym????????
typedef enum {
  oddsym = 1,
  identsym = 2,
  numbersym = 3,
  plussym = 4,
  minussym = 5,
  multsym = 6,
  slashsym = 7,
  ifelsym$8 = 8,
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
  elsesym$8 = 33,
} token_type;

// Global variables for PL/0 Instructions
int LIT = 1, OPR = 2, OPR_RTN = 0, OPR_ADD = 1, OPR_SUB = 2, OPR_MUL = 3,
    OPR_DIV = 4, OPR_EQL = 5, OPR_NEQ = 6, OPR_LSS = 7, OPR_LEQ = 8,
    OPR_GTR = 9, OPR_GEQ = 10, OPR_ODD = 11, LOD = 3, STO = 4, CAL = 5, INC = 6,
    JMP = 7, JPC = 8, SYS = 9, errorOffOrOn = 0;

// Function prototypes
void TERM();
void FACTOR();
void EXPRESSION();
int BLOCK();

// struture to store the lexeme and token
typedef struct {
  char string[MAX_ARRAY_LENGTH];
  int Token;
} subString;

/******************************************************/

// This function takes the input anf convert it to subString and store them to
// the subString array which is a sttructure array
int subStringCreater(char *inputArr, int sizeOfinputArr, subString *subString,
                     int sizeOfsubString, int stringIndex) { // function

  for (int i = 0; i < sizeOfinputArr; i++) {

    if (inputArr[i] >= 0 && inputArr[i] <= 32) // ASCII of invisivle charactors
    {
      continue;
    }
    if (isdigit(inputArr[i])) { // if the charactor is a digit

      stringIndex = 0;
      // add the digit to the string
      subString[sizeOfsubString].string[stringIndex++] = inputArr[i];

      // while the next charactors are digits add them to the string
      while (isdigit(inputArr[i + 1])) {
        i++;
        subString[sizeOfsubString].string[stringIndex++] = inputArr[i];
      }
      // add null terminator
      subString[sizeOfsubString].string[stringIndex] = '\0';

      // set stringIndex to 0 to use it for creating another string
      stringIndex = 0;
      // increment sizeOfsubString to create another string
      sizeOfsubString++;
    } else if (isalpha(inputArr[i])) { // identifier

      stringIndex = 0;

      // add the alphabet to the string
      subString[sizeOfsubString].string[stringIndex++] = inputArr[i];

      // while the next charactors are alphabet or digits add them to the string
      while (isalpha(inputArr[i + 1]) || isdigit(inputArr[i + 1])) {
        i++;
        subString[sizeOfsubString].string[stringIndex++] = inputArr[i];
      }

      // add null terminator
      subString[sizeOfsubString].string[stringIndex] = '\0';

      // set stringIndex to 0 to use it for creating another string
      stringIndex = 0;
      // increment sizeOfsubString to create another string
      sizeOfsubString++;
    } else if (((inputArr[i] == '<' && inputArr[i + 1] == '>') ||
                (inputArr[i] == '<' && inputArr[i + 1] == '=') ||
                (inputArr[i] == '>' && inputArr[i + 1] == '=') ||
                (inputArr[i] == ':' &&
                 inputArr[i + 1] == '='))) { // take care of <>   <=   >=   :=

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
    } else if ((inputArr[i] == '/' &&
                inputArr[i + 1] == '*')) { // handle comment

      stringIndex = 0;

      // Traversing in the array to see if */ exsists
      int j;
      for (j = i + 1 + 1; j < sizeOfinputArr; j++) {
        if (inputArr[j] == '*' && inputArr[j + 1] == '/') {
          // update the index to skip the comment
          i = j + 1;
          break;
        }
      }
      if (i < j + 1) { //  if */ did not find

        // creates two diffrent subStrings, one for the star and one for the
        // slash
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
    } else if (inputArr[i] != ' ' && inputArr[i] != '\t' &&
               inputArr[i] != '\n') { // taking care of sybmols and oprators

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

int isThisAnumber(char string[]) { // checks if the string is a number

  int sizeOfstring = strlen(string);

  for (int i = 0; i < sizeOfstring; i++) {
    if (!isdigit(string[i]))
      return 0;
  }
  return 1;
}

/******************************************************/

void tokenCreator(subString *subString, int sizeOfsubString) { // function

  for (int i = 0; i < sizeOfsubString; i++) {

    // plus
    if (strcmp(subString[i].string, "+") == 0) {
      subString[i].Token = plussym;
    }
    // minus
    else if (strcmp(subString[i].string, "-") == 0) {
      subString[i].Token = minussym;
    }
    // multiplication
    else if (strcmp(subString[i].string, "*") == 0) {
      subString[i].Token = multsym;
    }
    // slash
    else if (strcmp(subString[i].string, "/") == 0) {
      subString[i].Token = slashsym;
    }
    // odd
    else if (strcmp(subString[i].string, "odd") == 0) {
      subString[i].Token = oddsym;
    }
    // ifel
    // else if (strcmp(subString[i].string, "ifel") == 0)
    // {
    //   subString[i].Token = ifelsym;
    // }
    // equal
    else if (strcmp(subString[i].string, "=") == 0) {
      subString[i].Token = eqlsym;
    }
    // non equal
    else if (strcmp(subString[i].string, "<>") == 0) {
      subString[i].Token = neqsym;
    }
    // less than
    else if (strcmp(subString[i].string, "<") == 0) {
      subString[i].Token = lessym;
    }
    // less than or equal to
    else if (strcmp(subString[i].string, "<=") == 0) {
      subString[i].Token = leqsym;
    }
    // greater than
    else if (strcmp(subString[i].string, ">") == 0) {
      subString[i].Token = gtrsym;
    }
    // greater than or equal to
    else if (strcmp(subString[i].string, ">=") == 0) {
      subString[i].Token = geqsym;
    }
    // left parenthesis
    else if (strcmp(subString[i].string, "(") == 0) {
      subString[i].Token = lparentsym;
    }
    // right parenthesis
    else if (strcmp(subString[i].string, ")") == 0) {
      subString[i].Token = rparentsym;
    }
    // comma
    else if (strcmp(subString[i].string, ",") == 0) {
      subString[i].Token = commasym;
    }
    // semicolon
    else if (strcmp(subString[i].string, ";") == 0) {
      subString[i].Token = semicolonsym;
    }
    // period
    else if (strcmp(subString[i].string, ".") == 0) {
      subString[i].Token = periodsym;
    }
    // become
    else if (strcmp(subString[i].string, ":=") == 0) {
      subString[i].Token = becomessym;
    }
    // begin
    else if (strcmp(subString[i].string, "begin") == 0) {
      subString[i].Token = beginsym;
    }
    // end
    else if (strcmp(subString[i].string, "end") == 0) {
      subString[i].Token = endsym;
    }
    // if
    else if (strcmp(subString[i].string, "if") == 0) {
      subString[i].Token = ifsym;
    }
    // then
    else if (strcmp(subString[i].string, "then") == 0) {
      subString[i].Token = thensym;
    }
    // while
    else if (strcmp(subString[i].string, "while") == 0) {
      subString[i].Token = whilesym;
    }
    // do
    else if (strcmp(subString[i].string, "do") == 0) {
      subString[i].Token = dosym;
    }
    // call
    else if (strcmp(subString[i].string, "call") == 0) {
      subString[i].Token = callsym;
    }
    // const
    else if (strcmp(subString[i].string, "const") == 0) {
      subString[i].Token = constsym;
    }
    // var
    else if (strcmp(subString[i].string, "var") == 0) {
      subString[i].Token = varsym;
    }
    // procedure
    else if (strcmp(subString[i].string, "procedure") == 0) {
      subString[i].Token = procsym;
    }
    // write
    else if (strcmp(subString[i].string, "write") == 0) {
      subString[i].Token = writesym;
    }
    // read
    else if (strcmp(subString[i].string, "read") == 0) {
      subString[i].Token = readsym;
    }
    // else
    // else if (strcmp(subString[i].string, "else") == 0)
    // {
    //   subString[i].Token = elsesym;
    // }
    // number
    else if (isThisAnumber(subString[i].string)) {

      if (strlen(subString[i].string) <= MAX_DIGITS_LENGTH)
        subString[i].Token = numbersym;
      else
        subString[i].Token = -1; // Error : Numbers cannot exceed 5 digits
    }
    // idenfier
    else if (isalpha(subString[i].string[0]) ||
             isdigit(subString[i].string[0])) {
      if (strlen(subString[i].string) > MAX_CHARACTER_LENGTH) {
        subString[i].Token =
            -2; // Error : Identifiers cannot exceed 11 characters in length
      } else
        subString[i].Token = identsym;
    }

    else
      subString[i].Token = -3; // Error : Invalid Symbol
  }
}

/******************************************************/

// prints out the lexeme table
void LexemeTable(subString *subString, int sizeOfsubString) {

  // printf("Lexeme Table:\n");
  // fprintf(fp2, "Lexeme Table:\n"); // print to output file

  // printf("lexeme\t\ttoken type\n");
  // fprintf(fp2, "lexeme      token type\n"); // print to output file

  // prints out the lexemes
  for (int i = 0; i < sizeOfsubString; i++) {

    if (subString[i].Token > 0) { // check if token is valid

      // printf("%s", subString[i].string);
      // fprintf(fp2, "%s", subString[i].string); // print to output file

      // creat the space to build the lexeme table
      // int len = strlen(subString[i].string);
      // for (int j = 0; j < 12 - len; j++)
      // {
      //  printf(" ");
      //   //fprintf(fp2, " "); // print to output file
      // }
    }
    // prints the erorrs
    if (subString[i].Token == -1) {
      printf("Error : Number too long.\n");
      exit(1);
      // fprintf(fp2, "Error : Number too long.\n"); // print to output file
    } else if (subString[i].Token == -2) {
      printf("Error : Name too long.\n");
      exit(1);
      // fprintf(fp2, "Error : Name too long.\n"); // print to
    } else if (subString[i].Token == -3) {
      printf("Error : Invalid Symbols.\n");
      exit(1);
      // fprintf(fp2, "Error : Invalid Symbols.\n"); // print to output file
    }
    // else
    // {
    //   printf("%d\n", subString[i].Token);
    // fprintf(fp2, "%d\n", subString[i].Token); // print to output file
    // }
  }
}

/******************************************************/

// convering string to number
int str_to_int(const char *str) {
  int result = 0;
  int sign = 1; // To handle negative numbers

  // Check for a negative sign
  if (*str == '-') {
    sign = -1;
    str++; // Move to the next character
  }

  while (*str) {
    // Ensure the character is a digit
    if (*str < '0' || *str > '9') {
      printf("Invalid character detected: %c\n", *str);
      return 0; // Or handle the error as required
    }

    result = (result * 10) + (*str - '0');
    str++;
  }

  return result * sign;
}

/******************************************************/

// return the length of the number
int getLength(int num) {
  if (num == 0)
    return 1;
  int digits = 0;
  int length = 0;
  while (num > 0) {
    length++;
    num /= 10;
  }
  return length;
}

/******************************************************/

void error(int typeOfError) {
  switch (typeOfError) {
  case 1:
    printf("Error: Program must end with period.\n");
    break;
  case 2:
    printf("Error: Const, var, procedure, call and read keywords must be "
           "followed by "
           "identifier.\n");
    break;
  case 3:
    printf("Error: Symbol name has already been declared.\n");
    break;
  case 4:
    printf("Error: Constants must be assigned with =.\n");
    break;
  case 5:
    printf("Error: Constants must be assigned an integer value.\n");
    break;
  case 6:
    printf("Error: Constant, variable, and procedure declarations must be "
           "followed by a "
           "semicolon.\n");
    break;
  case 7:
    printf("Error: Undeclared identifier or out of scope %s\n",
           identArray[TOKEN].id);
    break;
  case 8:
    printf("Error: Only variable values may be altered.\n");
    break;
  case 9:
    printf("Error: Assignment statements must use :=.\n");
    break;
  case 10:
    printf("Error: Begin must be followed by end.\n");
    break;
  case 11:
    printf("Error: If must be followed by then.\n");
    break;
  case 12:
    printf("Error: While must be followed by do.\n");
    break;
  case 13:
    printf("Error: Condition must contain comparison operator.\n");
    break;
  case 14:
    printf("Error: Right parenthesis must follow left parenthesis.\n");
    break;
  case 15:
    printf("Error: Arithmetic equations must contain operands, parentheses, "
           "numbers, or symbols.\n");
    break;
  case 16:
    printf("program to long.\n");
    break;
  case 17:
    printf("missing semicolon.\n");
    break;
  case 18:
    printf("Call must be followed by procedure\n");
    break;
  default:
    printf("Error: Unknown error type.\n");
    break;
  }
  exit(1); // Exit the program with an error status
}

/******************************************************/

// function to generate code
void emit(int op, int L, int M) {
  if (cx > CODE_SIZE)
    error(16);
  else {

    text[cx].op = op;
    text[cx].L = L;
    text[cx].M = M;
    cx++;
  }
}

/******************************************************/

/*             Creat two arrays to store the tokens and identfiers


              - int token_array[] (size: sizeOftoken_arra) to store the tokens
              - stringHolder ident_array[] (size: sizeOfident_array) to store
   the identfiers

*/

void TokenListAndTokenArrayPopulat(subString *subString, int sizeOfsubString) {

  // printf("Token List:\n");
  // fprintf(fp2, "Token List:\n"); // print to output file

  for (int i = 0; i < sizeOfsubString; i++) {
    // prints valid tokens
    if (subString[i].Token > 0) {
      token_array[(sizeOftoken_arra)++] = subString[i].Token; //...........

      // printf("%d ", subString[i].Token);
      // fprintf(fp2, "%d ", subString[i].Token); // print to output file
    }
    if (subString[i].Token == 2 || subString[i].Token == 3) {
      if (subString[i].Token == 3)
        token_array[(sizeOftoken_arra)++] = str_to_int(subString[i].string);
      else {

        strcpy(identArray[(sizeOfidentArray)].id, subString[i].string);
        token_array[(sizeOftoken_arra)++] = sizeOfidentArray;
        (sizeOfidentArray)++;
      }

      // printf("%s ", subString[i].string);
      // fprintf(fp2, "%s ", subString[i].string); // print to output file
    }
  }
  // printf("\n");
  // fprintf(fp2, "\n"); // print to output file
}

/******************************************************/

void printSymbolTable() {
  printf("\nSymbol Table:\n\n");

  printf("Kind | Name        | Value | Level | Address | Mark\n");
  printf("---------------------------------------------------\n");

  for (int i = 0; i < sizeOfSymbolTable; i++) {

    printf("   %d |", symbolTable[i].kind);

    for (int j = 0; j < 12 - strlen(symbolTable[i].name); j++) {
      printf(" ");
    }

    printf("%s |     %d |", symbolTable[i].name, symbolTable[i].val);
    if (symbolTable[i].kind == 2 || symbolTable[i].kind == 3)
      printf("     %d |     %d |", symbolTable[i].level, symbolTable[i].addr);
    else
      printf("     - |     - |");
    printf("     %d\n", symbolTable[i].mark);
  }
}

/******************************************************/

// linear search through symbol table looking at name
// return index if found, -1 if not

int SYMBOLTABLECHECK(char *string) {

  // Iterate backwards through the symbol table.
  for (int seek = sizeOfSymbolTable - 1; seek >= 0; seek--) {

    // Compare the current symbol table entry's name with the input string.
    if (strcmp(symbolTable[seek].name, string) == 0) {

      // If a match is found, return the current index.
      return seek;
    }
  }

  // If no match is found, return -1.
  return -1;
}

/*****************************************************/

int isVarDuplicate(char *string) {

  // Iterate backwards through the symbol table.
  for (int seek = sizeOfSymbolTable - 1; seek >= 0; seek--) {

    // Compare the current symbol table entry's name with the input string.
    if (strcmp(symbolTable[seek].name, string) == 0 &&
        level == symbolTable[seek].level) {

      // If a match is found, return the current index.
      return 1;
    }
  }

  // If no match is found, return -1.
  return 0;
}

/******************************************************/

// return the int value of the current token
void GET_TOKEN() {
  // Increment the index to point to the next token.
  currentToken++;

  // Update the TOKEN with the next value from the token_array.
  TOKEN = token_array[currentToken];
}

/******************************************************/

// implement the symbol table
void ENTER(int kind, char *name, int value, int level, int addr) {

  symbol s;      //// Initialize a new symbol struct with the provided values.
  s.kind = kind; // Set the symbol's kind.
  strcpy(s.name, name); // Copy the name into the symbol's name field.
  s.val = value;        // Set the symbol's value.
  s.level = level;      // Initialize level to 0
  s.addr = addr;        // Set the symbol's address.
  s.mark = 0;           // Set the symbol's marker.

  // Insert the new symbol into the symbol table at the next available position.
  symbolTable[sizeOfSymbolTable] = s;

  // Increment the symbol table's size counter.
  sizeOfSymbolTable++;
}

/******************************************************/

void CONST_DECLARATION() {
  do {
    int number;
    char ident[12];

    // Get next token
    GET_TOKEN(); // srore the next token in TOKEN

    if (TOKEN != identsym) {
      error(2);
    }

    GET_TOKEN(); // Although it is not part of the Pseudocode code, because of
                 // the instruction of our Token_array, we need it. After the
                 // token of identsym we store the index of the identifier
                 // stored in identArray

    strcpy(ident, identArray[TOKEN].id);

    // we need to check if it is already in the symbol table
    if (SYMBOLTABLECHECK(ident) != -1) {
      error(3); // Error: Duplicate variable declaration
    }

    GET_TOKEN(); // srore the next token in TOKEN

    if (TOKEN != eqlsym) { // Error: constants must be assigned with =
      error(4);
    }

    GET_TOKEN(); // srore the next token in TOKEN

    if (TOKEN != numbersym) { // if the token is not a number
      error(5);
    }

    GET_TOKEN(); // Although it is not part of the Pseudocode code, because of
                 // the instruction of our Token_array we need it. In the token
                 // array, we saved the value of the const after the numbersym
                 // token.
    // now the token is the value of the const
    number = TOKEN;

    ENTER(1, ident, number, level, 0); // call symbol table for const

    GET_TOKEN(); // srore the next token in TOKEN

  } while (TOKEN == commasym);

  if (TOKEN != semicolonsym) { // Error: Missing semicolon
    error(6);
  }
  GET_TOKEN(); // get next token
}

/******************************************************/

int VAR_DECLARATION() {
  int numVars = 0; // vaiables counter

  do {

    numVars++; // count number of variabels

    char ident[12];

    GET_TOKEN();           // Get next token
    if (TOKEN != identsym) // if the token is not identifier
      error(2);

    //.....
    GET_TOKEN(); // to get the index of the identifier stored in identArray
    strcpy(ident, identArray[TOKEN].id);
    //.....

    // we need to check if it is already in the symbol table
    if (isVarDuplicate(ident) ==
        1)      
      error(3); // Error: Duplicate variable declaration

    GET_TOKEN(); // get next token

    ENTER(2, ident, 0, level, numVars + 2); // use 2 for var

  } while (TOKEN == commasym); // while the token is commasym

  if (TOKEN != semicolonsym) // Error: Missing semicolon
    error(6);

  GET_TOKEN(); // srore the next token in TOKEN

  return numVars; // return number of variables
}

/******************************************************/

void FACTOR() {

  // Handle identifiers
  if (TOKEN == identsym) {

    char ident[12]; // Buffer to store the identifier name

    // .....
    GET_TOKEN();                         // get the identifier name
    strcpy(ident, identArray[TOKEN].id); // Copy the identifier name
    // .....

    // Check if the identifier exists in the symbol table
    int symIdx = SYMBOLTABLECHECK(ident);
    if (symIdx == -1) {
      error(7); // Error: Undeclared identifier
    }

    // If the symbol is a constant, emit a LIT instruction
    if (symbolTable[symIdx].kind == 1) {
      emit(LIT, 0, symbolTable[symIdx].val);
    }
    // If the symbol is a var, emit a LOD instruction
    else if (symbolTable[symIdx].kind == 2) {
      emit(LOD, level - symbolTable[symIdx].level,
           symbolTable[symIdx].addr); // level set
    }
    GET_TOKEN(); // Move to the next token
  }
  // Handle numbers
  else if (TOKEN == numbersym) {
    // .....
    GET_TOKEN(); // get the value of the number
    // .....

    emit(LIT, 0, TOKEN); // Emit a LIT instruction with the number's value
    GET_TOKEN();         // Move to the next token
  }
  // Handle sub-expressions within parentheses
  else if (TOKEN == lparentsym) {
    GET_TOKEN();  // Move past the left parenthesis
    EXPRESSION(); // Process the expression inside the parentheses

    // Expect a right parenthesis after the expression
    if (TOKEN != rparentsym) {
      error(14); // Error: Right parenthesis missing
    }
    GET_TOKEN(); // Move to the next token
  }
  // If none of the above, it's an error
  else {
    error(15); // Error: Invalid factor
  }
}

/******************************************************/

/**
 * Processes a term in an expression.
 * A term is defined as a factor or a series of factors separated by
 * multiplication (*) or division (/) operators.
 */
void TERM() {
  // Process the first factor of the term.
  FACTOR();

  // Continue processing as long as there is a multiplication or division
  // symbol.
  while (TOKEN == multsym || TOKEN == slashsym) {
    // Handle multiplication.
    if (TOKEN == multsym) {
      GET_TOKEN(); // Move to the next token after the multiplication symbol.
      FACTOR();    // Process the next factor in the term.
      emit(OPR, 0, OPR_MUL); // Emit opcode for multiplication operation.
    }
    // Handle division.
    else if (TOKEN == slashsym) {
      GET_TOKEN(); // Move to the next token after the division symbol.
      FACTOR();    // Process the next factor in the term.
      emit(OPR, 0, OPR_DIV); // Emit opcode for division operation.
    }
  }
}

/******************************************************/

/**
 * Processes an expression according to the grammar rule:
 * expression ::= term { ("+" | "-") term }
 * An expression is a sequence of terms combined using addition or subtraction
 * operators.
 */
void EXPRESSION() {
  // Process the first term of the expression.
  TERM();

  // Continue processing as long as there is an addition or subtraction symbol.
  while (TOKEN == plussym || TOKEN == minussym) {
    // Handle addition.
    if (TOKEN == plussym) {
      GET_TOKEN();           // Move to the next token after the plus symbol.
      TERM();                // Process the next term in the expression.
      emit(OPR, 0, OPR_ADD); // Emit opcode for addition operation.
    }
    // Handle subtraction.
    else {                   // Implies TOKEN == minussym
      GET_TOKEN();           // Move to the next token after the minus symbol.
      TERM();                // Process the next term in the expression.
      emit(OPR, 0, OPR_SUB); // Emit opcode for subtraction operation.
    }
  }
}

/******************************************************/

/**
 * Processes a condition as defined by the grammar of the language:
    condition ::= "odd" expression | expression  rel-op  expression

 */
void CONDITION() {

  // Check if the condition is an 'odd' operation.
  if (TOKEN == oddsym) {
    GET_TOKEN();  // Move to the next token after 'odd'.
    EXPRESSION(); // Process the expression whose oddness is to be checked.
    emit(OPR, 0, OPR_ODD); // Emit opcode for the 'odd' operation.
  } else {
    // Process the first expression of a comparative condition.
    EXPRESSION();

    // Depending on the token, process the second part of the condition.
    if (TOKEN == eqlsym) { // Equality check
      GET_TOKEN();
      EXPRESSION();
      emit(OPR, 0, OPR_EQL);      // Emit opcode for equality comparison.
    } else if (TOKEN == neqsym) { // Inequality check
      GET_TOKEN();
      EXPRESSION();
      emit(OPR, 0, OPR_NEQ);      // Emit opcode for inequality comparison.
    } else if (TOKEN == lessym) { // Less-than check
      GET_TOKEN();
      EXPRESSION();
      emit(OPR, 0, OPR_LSS);      // Emit opcode for less-than comparison.
    } else if (TOKEN == leqsym) { // Less-than or equal-to check
      GET_TOKEN();
      EXPRESSION();
      emit(OPR, 0,
           OPR_LEQ); // Emit opcode for less-than or equal-to comparison.
    } else if (TOKEN == gtrsym) { // Greater-than check
      GET_TOKEN();
      EXPRESSION();
      emit(OPR, 0, OPR_GTR);      // Emit opcode for greater-than comparison.
    } else if (TOKEN == geqsym) { // Greater-than or equal-to check
      GET_TOKEN();
      EXPRESSION();
      emit(OPR, 0,
           OPR_GEQ); // Emit opcode for greater-than or equal-to comparison.
    } else {
      // If no valid comparison operator is found, raise an error.
      error(13); // Error: Invalid or missing comparison operator.
    }
  }
}

/******************************************************/

void STATEMENT() {

  // ident ":=" expression
  if (TOKEN == identsym) // if token is an identifier
  {

    char ident[12];

    GET_TOKEN(); // Get next token
    // now TOKEN is the index of the identifier stored in identArray
    strcpy(ident, identArray[TOKEN].id);

    int symIdx = SYMBOLTABLECHECK(ident);
    if (symIdx == -1) // make sure the identifier decalared
    {
      error(7); // Error: Undeclared identifier
    }
    if (symbolTable[symIdx].kind != 2) { // not a var
      error(8);
    }

    GET_TOKEN();

    if (TOKEN != becomessym) // if ident is not follwoed by :=
    {
      error(9);
    }

    GET_TOKEN();

    EXPRESSION(); // proess the expression after the :=

    emit(STO, level - symbolTable[symIdx].level,
         symbolTable[symIdx].addr); // code: sto 0 M or 4 0 M  level set

    return;
  }
  //"call" ident
  if (TOKEN == callsym) {

    GET_TOKEN();
    if (TOKEN != identsym) {
      error(2); // Identifier must be followed by a call
    }
    GET_TOKEN(); // TOKEN is index at this point

    int indexIfProcedureName = TOKEN;

    // check if identifier is declared
    int i = SYMBOLTABLECHECK(identArray[TOKEN].id);

    if (i == -1) {
      error(7); // Invalid symbol
    }

    if (symbolTable[i].kind == 3) { // if identifier is a procedure

      emit(CAL, level - symbolTable[i].level,
           indexIfProcedureName); // save the index of the idenfifier, help us
                                  // to find its M in the callArray
    } else {
      error(18);
    }

    GET_TOKEN(); // get the next token

    return;
  }
  //"begin" statement { ";" statement } "end"
  if (TOKEN == beginsym) // if statement starts with begin
  {
    do {
      GET_TOKEN();
      STATEMENT();                   // process the statement afte the begin
    } while (TOKEN == semicolonsym); // if there is a semicolon after the
                                     // statement process the next statement

    if (TOKEN != endsym) // if the token is not end
    {
      error(10);
    }
    GET_TOKEN();
    return;
  }
  // "if" condition "then" statement
  if (TOKEN == ifsym) // if token is if
  {
    GET_TOKEN();
    CONDITION(); // process the condition afte the if

    int jpcIdx = cx; // cureent text(code) index
    emit(JPC, 0, 0); // JPC 0 0 or 8 0 0

    if (TOKEN != thensym) {
      error(11);
    }
    GET_TOKEN();
    STATEMENT(); // process the statement after the then

    text[jpcIdx].M = cx * 3; // set M for JPC

    return;
  }
  //"while" condition "do" statement
  if (TOKEN == whilesym) // if token is while
  {
    GET_TOKEN();
    int loopIdx = cx; //// cureent text(code) index

    CONDITION(); // process the condition afte the while

    if (TOKEN != dosym) // if the token is not do
    {
      error(12);
    }
    GET_TOKEN();

    int jpcIdx = cx; // cureent text(code) index

    emit(JPC, 0, 0); // JPC 0 0 or 8 0 0

    STATEMENT(); // process the statement after the do

    emit(JMP, 0, loopIdx * 3); // jmp 0 loopIdx or 7 0 loopIdx

    text[jpcIdx].M = cx * 3; // set M for JPC

    return;
  }

  if (TOKEN == readsym) // if the token is read
  {

    char ident[12];

    GET_TOKEN();
    if (TOKEN != identsym) // make sure that right after read is an identifier
    {
      error(2);
    }

    GET_TOKEN();

    // make sure that the identifier already decalared
    strcpy(ident, identArray[TOKEN].id);
    int symIdx = SYMBOLTABLECHECK(ident);
    if (symIdx == -1) {
      error(7); // Error: Undeclared identifier
    }
    if (symbolTable[symIdx].kind != 2) // make sure that the identifier is a var
    {                                  // not a var
      error(8);
    }

    GET_TOKEN();
    emit(9, 0, 2); // gSYS 0 2 or 9 0 2
    emit(STO, level - symbolTable[symIdx].level,
         symbolTable[symIdx].addr); // STO,0, symbolTable[symIdx].addr
    return;
  }

  if (TOKEN == writesym) // if the token is write
  {
    GET_TOKEN();
    EXPRESSION();  // proccess the expression after write
    emit(9, 0, 1); // SYS 0 1 or 9 0 1
    return;
  }
}

/******************************************************/

void PROC_DECL() {

  GET_TOKEN();

  if (TOKEN != identsym) {
    error(2);
  }

  GET_TOKEN(); // index of identifire

  int i = SYMBOLTABLECHECK(identArray[TOKEN].id);

  if (i != -1)
    error(3);

  // strcpy(callArray[sizeOfCallArray].name, identArray[TOKEN].id);
  // sizeOfCallArray++;

  ENTER(3, identArray[TOKEN].id, 0, level, TOKEN); // OPTIONAL: we used the INC

  int indexOfProcInSymboltable = sizeOfSymbolTable - 1;

  GET_TOKEN();

  if (TOKEN != semicolonsym) {
    error(6);
  }
  GET_TOKEN();

  // fix the 666 there
  int pro_addr = BLOCK();

  symbolTable[indexOfProcInSymboltable].addr = pro_addr;

  // populate the callArray
  strcpy(callArray[sizeOfCallArray].name,
         symbolTable[sizeOfSymbolTable - 1].name);

  callArray[sizeOfCallArray].destination =
      symbolTable[sizeOfSymbolTable - 1].addr;

  sizeOfCallArray++;

  if (TOKEN != semicolonsym) {
    error(17); // the procedure has to end with a semicolon
  } else {
    emit(OPR, 0, 0);
  }
  GET_TOKEN();
}
/* $$$$$$$$$$$$$$$$$ */

int BLOCK() {

  level++;

  int prev_sx = sizeOfSymbolTable;

  int space = 3;

  int jmpaddr = cx; // current text(code) index

  emit(JMP, 0, 666);
  do {
    // if token is const
    if (TOKEN == constsym) {
      CONST_DECLARATION(); // call function
    }

    // if token is var
    if (TOKEN == varsym) {
      space += VAR_DECLARATION(); // call function

      // emit(INC, 0, numVars + 3); // create spaces for RN, SL, DL and vars
    }

    /*  $$$$$$$$$$$$$$$$$ */
    // while token is procedure
    while (TOKEN == procsym) {

      PROC_DECL();
    }
  } while (TOKEN == procsym || TOKEN == varsym || TOKEN == constsym);
  /*  $$$$$$$$$$$$$$$$$ */

  text[jmpaddr].M = cx * 3; // set M for JMP

  int proc_addr = jmpaddr; // proc_addr is the address of the procedure

  emit(INC, 0, space); // INC 0 numVars + 3

  STATEMENT(); // call function

  sizeOfSymbolTable = prev_sx;

  level--;

  return proc_addr;
}

/******************************************************/

void PROGRAM() {

  GET_TOKEN();

  BLOCK(); // call function

  if (TOKEN != periodsym) { // if the token is not period
    error(1);
  }
  emit(SYS, 0, 3); // Halt

  //
  for (int j = 0; j < sizeOfCallArray; j++) {
    printf("&&&&&&&&&&&&&&&&&&&&&&&&&& %s : %d\n", callArray[j].name,
           callArray[j].destination);
  }
  for (int i = 0; i < cx; i++) {
    if (text[i].op == 5) {
      printf("here\n");
      for (int j = 0; j < sizeOfCallArray; j++) {
        if (strcmp(identArray[text[i].M].id, callArray[j].name) == 0)
          text[i].M = callArray[j].destination * 3;
      }
    }
  }
}

/******************************************************/

void codePrinter() {

  printf("Line    OP    L    M\n");
  for (int i = 0; i < cx; i++) {
    for (int j = 0; j < 3 - getLength(i); j++)
      printf(" ");
    printf("%d    ", i);
    switch (text[i].op) {
    case 1:
      printf("LIT");
      break;
    case 2:
      printf("OPR");
      break;
    case 3:
      printf("LOD");
      break;
    case 4:
      printf("STO");
      break;
    case 5:
      printf("CAL");
      break;
    case 6:
      printf("INC");
      break;
    case 7:
      printf("JMP");
      break;
    case 8:
      printf("JPC");
      break;
    case 9:
      printf("SYS");
      break;
    default:
      printf("err");
    }
    printf("    %d    %d\n", text[i].L, text[i].M);
    fprintf(fp2, "%d %d %d\n", text[i].op, text[i].L, text[i].M);
  }
}

/******************************************************/

int main(int argc, char *argv[]) {

  if (argc < 2) { // checks if there is a file
    printf("Error : please include the file name");
    return 1;
  }

  FILE *fp = fopen(argv[1], "r"); // opens the file

  if (fp == NULL) { // checks if the file is valid
    printf("Error : cannot open file");
    return 1;
  }

  for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
    callArray[i].destination = -1;
  // define the inputArr array and subString array
  char inputArr[MAX_ARRAY_LENGTH];
  subString subString[MAX_ARRAY_LENGTH];
  // define a variable to track the size of the inputArr array
  int sizeOfinputArr = 0;
  // define a variable to track the size of the subString array
  int sizeOfsubString = 0;

  char c;
  // read character input by fscanf
  while (fscanf(fp, "%c", &c) != EOF) {
    // reads the file and store chacracter by chacracter in the arr
    inputArr[sizeOfinputArr] = c;
    sizeOfinputArr++;
  }

  // close fp
  fclose(fp);

  // set the end of the inputArr to null
  inputArr[sizeOfinputArr] = '\0';

  // open a file in w mode
  fp2 = fopen("elf.text", "w");

  // arrTracker
  // arrTracker is the last index of the array
  int stringIndex = 0;

  // call function to creat subStrings and return the # of the strings
  sizeOfsubString =
      subStringCreater(inputArr, sizeOfinputArr, subString, sizeOfsubString,
                       stringIndex); // call function

  // call function to generate token for eac subString
  tokenCreator(subString, sizeOfsubString); // call function

  // printf("\n\n");
  // fprintf(fp2, "\n\n"); // print to file

  // call function to print the lexeme table
  LexemeTable(subString, sizeOfsubString);

  // call function to populating the token_array
  TokenListAndTokenArrayPopulat(subString, sizeOfsubString);

  // print the token array
  // printf("Token Array:\n");
  // for (int i = 0; i < sizeOftoken_arra; i++)
  // {
  //   printf("%d ", token_array[i]);
  // }

  // printf("\n\n");

  // add jump 0 3 INC
  // emit(JMP, 0, 3);
  PROGRAM(); // call function

  // print the source code
  printf("Source Program:\n");
  // fprintf(fp2, "Source Program:\n"); // print to file
  for (int i = 0; i < sizeOfinputArr; i++) {
    printf("%c", inputArr[i]);
    // fprintf(fp2, "%c", inputArr[i]); // print to file
  }
  printf("\n\n");

  printf("No errors, program is syntactically correct\n\n");
  codePrinter(); // print the code (op l m) to the consol and elf.text

  // setting marks to 1
  // for (int i = 0; i < sizeOfSymbolTable; i++)
  //   symbolTable[i].mark = 1;     ?????????????

  // printSymbolTable(); // print the symbol table to the consol

  // close fp2

  fclose(fp2);

  return 0;
}
