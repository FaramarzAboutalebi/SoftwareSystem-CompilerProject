#include <stdio.h>
#include <ctype.h>
#include <string.h>

//??????????????????
#define MAX_ARRAY_LENGTH 500 
#define MAX_DIGITS_LENGTH 5
#define MAX_CHARACTER_LENGTH 11

typedef enum { 
skipsym = 1, 
identsym = 2, 
numbersym = 3,  
plussym = 4,  //...
minussym = 5, //...
multsym = 6, //...
slashsym = 7, //...
ifelsym = 8,  //...
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

/******************************************************/

typedef struct {
  char string[MAX_ARRAY_LENGTH];
  int Token;
} subString;

/******************************************************/

//this function takes input array and index and if that element is space or tab or new line, it will skip that element and traverse to get to a ont space, non tab and non new line element

int indexOfNextVisibleElement(char input[], int index) { // function
  

  if(!(input[index+1] >=0 && input[index+1] <= 32)) { // if that element is not invisble
    return index+1;
  }
  else if(index < strlen(input))
  return indexOfNextVisibleElement(input, index+1);

  return -1;
  
}

/******************************************************/
    
int subStringCreater(char * inputArr, int sizeOfinputArr, subString * subString, int sizeOfsubString, int stringIndex){   // function

    for(int i = 0; i < sizeOfinputArr; i++){


      if(inputArr[i] >=0 && inputArr[i] <= 32)// ASCII of invisivle charactors
      {
        continue;
      }
    else if(isalpha(inputArr[i]) || isdigit(inputArr[i])){// identifier and number
      stringIndex = 0; //.....
      subString[sizeOfsubString].string[stringIndex++] = inputArr[i];
      
      while(isalpha(inputArr[i+1]) || isdigit(inputArr[i+1])){
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
    else if( ((inputArr[i] == '<' && inputArr[indexOfNextVisibleElement(inputArr, i)] == '>') || (inputArr[i] == '<' && inputArr[indexOfNextVisibleElement(inputArr, i)] == '=') || (inputArr[i] == '>' && inputArr[indexOfNextVisibleElement(inputArr, i)] == '=') || (inputArr[i] == ':' && inputArr[indexOfNextVisibleElement(inputArr, i)] == '=')) && indexOfNextVisibleElement(inputArr, i) != -1){ // take care of <>   <=   >=   :=
      stringIndex = 0; //.....
      subString[sizeOfsubString].string[stringIndex++] = inputArr[i];
      subString[sizeOfsubString].string[stringIndex++] = inputArr[indexOfNextVisibleElement(inputArr, i)];
      // add null terminator
      subString[sizeOfsubString].string[stringIndex] = '\0'; 

      //update the index to last used element
      i = indexOfNextVisibleElement(inputArr, i);

      // set stringIndex to 0 to use it for creating another string
      stringIndex = 0;
      // increment sizeOfsubString to create another string
      sizeOfsubString++;
      
    }
    else if((inputArr[i] == '/' && inputArr[indexOfNextVisibleElement(inputArr, i)] == '*') && indexOfNextVisibleElement(inputArr, i) != -1){// handle comment

      stringIndex = 0; //.....
      // we travee in the inputArr to figure out if there is a star and slash. If so, it means we have to skip the contents of the comment
      for(int j = indexOfNextVisibleElement(inputArr, i)+1; j <  sizeOfinputArr; j++){
        if(inputArr[j] == '*' && inputArr[indexOfNextVisibleElement(inputArr, j)] == '/')
          //update the index to skip the comment
          i = indexOfNextVisibleElement(inputArr, j);
        
      }
      
    }
    else if(inputArr[i] != ' ' && inputArr[i] != '\t' && inputArr[i] != '\n'){

      
      
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
  
}//end

/******************************************************/
int isThisAnumber(char string[]){
  int sizeOfstring = strlen(string);
  if(sizeOfstring > 5) sizeOfstring = 5;
  for(int i = 0; i < sizeOfstring; i++){
    if(!isdigit(string[i]))
      return 0;
  }
  return 1;
}

/******************************************************/

void tokenCreator(subString * subString, int sizeOfsubString){  //function

  for(int i =0 ; i < sizeOfsubString; i++){

    //plus
    if(strcmp(subString[i].string, "+") == 0){
      subString[i].Token = plussym;
    }
    //minus
    else if(strcmp(subString[i].string, "-") == 0){
      subString[i].Token = minussym;
    }
    //multiplication
    else if(strcmp(subString[i].string, "*") == 0){
      subString[i].Token = multsym;
    }
    //slash
    else if(strcmp(subString[i].string, "/") == 0){
      subString[i].Token = slashsym;
    }
    //ifel
    else if(strcmp(subString[i].string, "ifel") == 0){//?????
      subString[i].Token = ifelsym;
    }
    //equal
    else if(strcmp(subString[i].string, "=") == 0){//???
      subString[i].Token = eqlsym;
    }
    //non equal
    else if(strcmp(subString[i].string, "<>") == 0){
      subString[i].Token = neqsym;
    }
    //less than
    else if(strcmp(subString[i].string, "<") == 0){
      subString[i].Token = lessym;
    }
    //less than or equal to
    else if(strcmp(subString[i].string, "<=") == 0){
      subString[i].Token = leqsym;
    }
    //greater than
    else if(strcmp(subString[i].string, ">") == 0){
      subString[i].Token = gtrsym;
    }
    //greater than or equal to
    else if(strcmp(subString[i].string, ">=") == 0){
      subString[i].Token = geqsym;
    }
    //left parenthesis
    else if(strcmp(subString[i].string, "(") == 0){
      subString[i].Token = lparentsym;
    }
    //right parenthesis
    else if(strcmp(subString[i].string, ")") == 0){
      subString[i].Token = rparentsym;
    }
    //comma
    else if(strcmp(subString[i].string, ",") == 0){
      subString[i].Token = commasym;
    }
    //semicolon
    else if(strcmp(subString[i].string, ";") == 0){
      subString[i].Token = semicolonsym;
    }
    //period
    else if(strcmp(subString[i].string, ".") == 0){
      subString[i].Token = periodsym;
    }
    // become
    else if(strcmp(subString[i].string, ":=") == 0){
      subString[i].Token = becomessym;
    }
    //begin
    else if(strcmp(subString[i].string, "begin") == 0){
      subString[i].Token = beginsym;
    }
    //end
    else if(strcmp(subString[i].string, "end") == 0){
      subString[i].Token = endsym;
    }
    //if
    else if(strcmp(subString[i].string, "if") == 0){
      subString[i].Token = ifsym;
    }
    //then
    else if(strcmp(subString[i].string, "then") == 0){
     subString[i].Token = thensym;
    }
    //while
    else if(strcmp(subString[i].string, "while") == 0){
      subString[i].Token = whilesym;
    }
    //do
    else if(strcmp(subString[i].string, "do") == 0){
      subString[i].Token = dosym;
    }
    //call
    else if(strcmp(subString[i].string, "call") == 0){
      subString[i].Token = callsym;
    }
    //const
    else if(strcmp(subString[i].string, "const") == 0){
      subString[i].Token = constsym;
    }
    //var
    else if(strcmp(subString[i].string, "var") == 0){
      subString[i].Token = varsym;
    }
    //procedure
    else if(strcmp(subString[i].string, "procedure") == 0){
      subString[i].Token = procsym;
    }
    //write
    else if(strcmp(subString[i].string, "write") == 0){
      subString[i].Token = writesym;
    }
    //read
    else if(strcmp(subString[i].string, "read") == 0){
      subString[i].Token = readsym;
    }
    //else
    else if(strcmp(subString[i].string, "else") == 0){
      subString[i].Token = elsesym;
    }
    //number
      
    else if(isThisAnumber(subString[i].string)){

      if(strlen(subString[i].string) <= MAX_DIGITS_LENGTH)
        subString[i].Token = numbersym;
      else
        subString[i].Token = -1;//Error : Numbers cannot exceed 5 digits

      
    }
    //idenfier
    else if(isalpha(subString[i].string[0]) || isdigit(subString[i].string[0])){
      if(isdigit(subString[i].string[0]))
        subString[i].Token = -2;//Error : Identifiers cannot begin with a digit
      else if(strlen(subString[i].string) > MAX_CHARACTER_LENGTH)
        subString[i].Token = -3;//Error : Identifiers cannot exceed 11 characters in length
      else
        subString[i].Token = identsym;
    }
    
    else
      subString[i].Token = -4;// Error : Invalid Symbol
    
      

  }

}


/******************************************************/

void LexemeTable(subString * subString, int sizeOfsubString){
  printf("Lexeme Table:\n"); //..............................................
  printf("lexeme\t\ttoken type\n");
  for(int i = 0; i < sizeOfsubString; i++){
    printf("%s\t\t\t", subString[i].string);
    if(subString[i].Token == -1)
      printf("Error : Numbers cannot exceed 5 digits\n");
    else if(subString[i].Token == -2)
      printf("Error : Identifiers cannot begin with a digit\n");
    else if(subString[i].Token == -3)
      printf("Error : Identifiers cannot exceed 11 characters in length\n");
    else if(subString[i].Token == -4)
      printf("Error : Invalid Symbol\n");
    else
      printf("%d\n", subString[i].Token);
      
  }
  
}

/******************************************************/
void TokenList(subString * subString, int sizeOfsubString){
  
  printf("Token List:\n"); 
  for(int i = 0; i < sizeOfsubString; i++){
    printf("%d ", subString[i].Token);
    if(subString[i].Token == 2 || subString[i].Token == 3){
      printf("%s ", subString[i].string);
    }
  }
  
}
/******************************************************/

int main( int argc, char *argv[]){

  if(argc < 2){
    printf("Error : please include the file name");
    return 1;
  }
  
  FILE *fp = fopen(argv[1], "r");

  if(fp == NULL){
    printf("Error : cannot open file");
    return 1;
  
  }

  char inputArr[MAX_ARRAY_LENGTH];
  subString subString[MAX_ARRAY_LENGTH];
  int sizeOfinputArr = 0;
  int sizeOfsubString = 0;
  char c;
  //read character input by fscanf
  while(fscanf(fp, "%c", &c) != EOF){
    //reads the file and store chacracter bt chacracter in the arr
    inputArr[sizeOfinputArr] = c;
    sizeOfinputArr++;
  }
  
  //set the end of the inputArr to null
  inputArr[sizeOfinputArr] = '\0';
  
  //arrTracker
  //arrTracker is the last index of the array
  int stringIndex = 0;

  //creat subStrings and return the # of the strings
  sizeOfsubString = subStringCreater(inputArr, sizeOfinputArr, subString, sizeOfsubString, stringIndex);// call function

  //generate token for eac subString
  tokenCreator(subString, sizeOfsubString); // call function

  //print the lexeme table
  LexemeTable(subString, sizeOfsubString);

  //print the token list
  TokenList(subString, sizeOfsubString);

  


  return 0;
  
}