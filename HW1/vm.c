//Faramarz Aboutalebi

#include <stdio.h>
#include <stdlib.h>


//Constant Values: ARRAY_SIZE is 512
#define ARRAY_SIZE 512

//create the PAS
int pas[ARRAY_SIZE];


//Function Prototypes
//Function to print the instructions and PAS
void print_execution(char *opname, int pc0, int pc, int bp, int sp, int *pas, int *AR, int textIndexTrack, int ARtrack){
  
  // print out instruction and registers
  
  printf("    %s  ", opname);

  int values[5] = {pas[pc0+1], pas[pc0+2], pc, bp, sp};
  for(int i = 0; i < 5; i++) {
      if(values[i] < 10) {
          printf("%d   ", values[i]);
      }
      else if(values[i] < 100) {
          printf("%d  ", values[i]);
      }
      else {
          printf("%d ", values[i]);
      }
  }

  //index tracker for AR
  int j = 0;
  for(int i = textIndexTrack+1; i <= sp; i++){
    
    printf("%d  ", pas[i]);
    if(bp>textIndexTrack+1 && i == AR[j]-1 && sp >= AR[j] && j<= ARtrack)
    {
      printf("|   ");
      j++;
    }
      
  }printf("\n");
  
}

//Find base L levels down
int base(int BP, int L)
{
	int arb = BP;	// arb = activation record base
	while ( L > 0)     //find base L levels down
	{
		arb = pas[arb];
		L--;
	}
	return arb;
}


int main(int argc, char* argv[]){
  

  // Check if the number of command line arguments is less than 2 (i.e., no file name provided)
  if(argc < 2){
    printf("No file name provided.\n");
    return 1;
  }

  // Store the file name from command line arguments
  char *fileName = argv[1];
  // Open the file in read mode
  FILE * fp = fopen(fileName, "r");

  // Check if the file was opened successfully
  if(fp == NULL){
    printf("File not found.\n");
    return 1;
  }

  
  //Initial process address space values are all zero
  for(int i = 0; i < ARRAY_SIZE; i++){
     pas[i] = 0;
  }
  
  //track the end of the text segement               
  int textIndexTrack = -1;                
  
  // read number of lines in file and skip the whilte spaces and new line
  int c;
  while((fscanf(fp, "%d", &c)) != EOF){
      textIndexTrack++;
      pas[textIndexTrack] = c;
  }

  //close the file
  fclose(fp);
  
  //now it is time to build the stack segment of the pass
  // textIndexTrack is the last index of the text segment
  // so we increase it by one and save it into pc to get acces to the base of
  //strucutre vairable
  // insruction IR;
  int bp = textIndexTrack + 1;
  int pc = 0;
  int sp = textIndexTrack;

  //craete an arrray to track activation records
  int AR[ARRAY_SIZE];
  int ARtrack = -1;

  printf("                 PC  BP  SP  stack\n");
  printf("Initial values:	 %d	 %d  %d\n\n", pc, bp, sp);
  
  
  while(1){

    if(pas[pc] == 1){// LIT
      
      int pc0 = pc;
      //after fetch
      pc += 3;
      //after execute
      sp += 1;
      pas[sp] = pas[pc0 + 2];//pas[sp] = M;
      
      print_execution("LIT", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
      
    }
    else if(pas[pc] == 2){//OPR
      
      if(pas[pc+2] == 0){//RTN
        
        int pc0 = pc;
        sp = bp - 1;
        bp = pas[sp + 2];
        //remove the activation record
        ARtrack--;
        pc = pas[sp + 3];
        print_execution("RTN", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
      }
      else if(pas[pc + 2] == 1){//ADD
        
        int pc0 = pc;
        //after fetch
        pc += 3;
        //after execute
        pas[sp - 1] = pas[sp - 1] + pas[sp];
        sp -= 1;
        print_execution("ADD", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
      }
      else if(pas[pc+2] == 2){//SUB

        int pc0 = pc;
        //after fetch
        pc += 3;
        //after execute
        pas[sp - 1] = pas[sp - 1] - pas[sp];
        sp -= 1;
        print_execution("SUB", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
      }
      else if(pas[pc+2] == 3){//MUL

        int pc0 = pc;
        //after fetch
        pc += 3;
        //after execute
        pas[sp - 1] = pas[sp - 1] * pas[sp];
        sp -= 1;
        print_execution("MUL", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
      }
      else if(pas[pc+2] == 4){//DIV

        int pc0 = pc;
        //after fetch
        pc += 3;
        //after execute
        pas[sp - 1] = pas[sp - 1] / pas[sp];
        sp -= 1;
        print_execution("DIV", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
        
      }
      else if(pas[pc+2] == 5){//EQl

        int pc0 = pc;
        //after fetch
        pc += 3;
        //after execute
        //check if the top of the stack is equal to the element at sp -1
        if(pas[sp - 1] == pas[sp]){
          pas[sp - 1] = 1; // they are equal
          sp -= 1;
        }else{
           pas[sp - 1] = 0;// they are not equal
          sp -= 1;
        }
        
        print_execution("EQl", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
      }
      else if(pas[pc+2] == 6){//NEQ
         //check if the top of the stack is ***not*** equal to the element at sp -1
        int pc0 = pc;
        //after fetch
        pc += 3;
        //after execute
        if(pas[sp - 1] != pas[sp]){
          pas[sp - 1] = 1; // they are not equal
          sp -= 1;
        }else{
          pas[sp - 1] = 0; // they are equal
          sp -= 1;
        }

        print_execution("NEQ", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
      }
      else if(pas[pc+2] == 7){ //LSS

        int pc0 = pc;
        //after fetch
        pc += 3;
        //after execute
        //check if the element at sp -1 is less than the top of the stack
        if(pas[sp - 1] < pas[sp]){
          pas[sp - 1] = 1; // less
          sp -= 1;
        }else{
          pas[sp - 1] = 0; // not less
          sp -= 1;
        }
        
        print_execution("LSS", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
          
      }
      else if(pas[pc+2] == 8){//LEQ

        int pc0 = pc;
        //after fetch
        pc += 3;
        //after execute
        //check if the element at sp -1 is less or equal to the top of the stack
        if(pas[sp - 1] <= pas[sp]){
          pas[sp - 1] = 1; // less or equal
          sp -= 1; 
        }else{
          pas[sp - 1] = 0; // not less or equal
          sp -= 1;
        }
        
        print_execution("LEQ", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
      }
      else if(pas[pc+2] == 9){//GTR

        int pc0 = pc;
        //after fetch
        pc += 3;
        
        //after execute
        //check if the element at sp -1 is grater than the top of the stack
        if(pas[sp - 1] > pas[sp]){
          pas[sp - 1] = 1; // greater
          sp -= 1;
        }else{
          pas[sp - 1] = 0; // not greater
          sp -= 1;
        }
      
        print_execution("GTR", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack

      }
      else if(pas[pc+2] == 10){//GEQ

        int pc0 = pc;
        //after fetch
        pc += 3;
        
        //after execute
        //check if the element at sp -1 is grater or equal to the top of the stack
        if(pas[sp - 1] >= pas[sp]){
          pas[sp - 1] = 1; // greater
          sp -= 1;
        }else{
          pas[sp - 1] = 0; // not greater
          sp -= 1;
        }
        
        print_execution("GEQ", pc0, pc, bp, sp, pas,AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
      }
      else if(pas[pc+2] == 11){//odd
        int pc0 = pc;
        //after fetch
        pc += 3;
        //after execute
        //check if the element at sp is odd
        if(pas[sp] % 2 == 1){
          pas[sp] = 1; // odd
        }else{
          pas[sp] = 0; // odd
        }
          
        print_execution("ODD", pc0, pc, bp, sp, pas,AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
      }
    }  
    else if(pas[pc] == 3){//LOD

      int pc0 = pc;
      //after fetch
      pc += 3;
        
      //after execute
      sp += 1;
      pas[sp] = pas[base(bp, pas[pc0 + 1]) + pas[pc0 + 2]]; //pas[sp] = pas[base(bp, L) + M];

      print_execution("LOD", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
      
    }
    else if(pas[pc] == 4){//STO

      int pc0 = pc;
      //after fetch
      pc += 3;
        
      //after execute
      pas[base(bp, pas[pc0 + 1]) + pas[pc0 + 2]] = pas[sp]; //pas[base(bp, L) + M] = pas[sp];
			sp -= 1;  
      print_execution("STO", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
      
    }
    else if(pas[pc] == 5){//CAL
      
      int pc0 = pc;
      
      //After FETCH
      pc += 3;
      pas[sp + 1] = base(bp, pas[pc + 1]); //pas[sp + 1] = base(bp, L); // static link (SL)
      pas[sp + 2] = bp; // dynamic link (DL)
	    pas[sp + 3] = pc; // return address (RA)
      bp = sp + 1;
      //record activation record
      ARtrack++;
      AR[ARtrack] = bp;
      
      //After EXECUTE
      pc = pas[pc0 + 2];//pc = M;

      print_execution("CAL", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
      
    }
    else if(pas[pc] == 6){//INC
      
      int pc0 = pc;
      //after fetch
      pc += 3;
        
      //after execute
      sp += pas[pc0 + 2];//sp += M;

      print_execution("INC", pc0, pc, bp, sp, pas,AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
      
    }
    else if(pas[pc] == 7){//JMP 

      int pc0 = pc;
      pc = pas[pc + 2]; //pc = M;
      
      print_execution("JMP", pc0, pc, bp, sp, pas,AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
      
    }
    else if(pas[pc] == 8){//JPC   

      int pc0 = pc;
      
      if(pas[sp] == 0){
        pc = pas[pc + 2]; //pc = M;
      } 
      else{
        pc += 3;
      }
      
      sp = sp - 1;

      print_execution("JPC", pc0, pc, bp, sp, pas,AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
      
    }
    else if(pas[pc] == 9){//SYS
      
      if(pas[pc+2] == 1){//M == 1

        int pc0 = pc;
        
        printf("Output result is: %d\n", pas[sp]);
        sp -= 1;

        pc += 3;
        print_execution("SYS", pc0, pc, bp, sp, pas,AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
      }else if(pas[pc+2] == 2){ //M == 2
       
        int pc0 = pc;
        printf("Please Enter an Integer: ");
        sp += 1;
        
        scanf("%d", &pas[sp]);
        pc+=3;
        print_execution("SYS", pc0, pc, bp, sp, pas,AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
      }else if(pas[pc+2] == 3){ //M == 3
        
        int pc0 = pc;
        pc += 3;
        print_execution("SYS", pc0, pc, bp, sp, pas, AR, textIndexTrack, ARtrack);// calling funciton to print the instruction and stack
        
        return 0 ;
      }
      
      
      
      
    }else{//invalid opcode
      printf("Invalid input\n");
      printf("%d\n", pas[pc-1]);
      printf("%d\n", pas[pc]);
      printf("%d\n", pas[pc+1]);
      exit(1);
    }

  }
  
  
  
  return 0;
  
  
}









  