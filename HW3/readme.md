# README: Recursive Descent Parser and Intermediate Code Generator for tiny PL/0

## Introduction
This README document details the implementation of HW3, which is a Recursive Descent Parser and Intermediate Code Generator for the tiny PL/0 language. The objective is to accurately parse PL/0 programs, identify syntax errors, and generate intermediate code for execution on a virtual machine.

## Features

- **Parsing PL/0 Programs**: Processes tokens from the Scanner (HW2) and analyzes them against the PL/0 grammar.
- **Error Handling**: Detects and reports lexical and syntax errors, halting execution on the first error encountered.
- **Symbol Table Generation**: Creates a table of all variables and constants used in the PL/0 program.
- **Intermediate Code Generation**: Outputs assembly code for a predefined virtual machine (HW1) if the program is syntactically correct.

## PL/0 Program Example

```pl0
var x, y;
begin
    x := y * 2;
end.
```

## Component Descriptions
- **Parser/Code Generator:** Accepts tokens from the Scanner and validates them against the PL/0 grammar. Populates the Symbol Table and generates virtual machine code.
- **Error Reporting:** On error detection, the process is halted and an error message is displayed.

 ## Output Specifications
- **Error Output:** If an error is found, it is displayed in the format Error : <error message>.
- **Success Output:** Outputs the assembly code for the virtual machine and the symbol table.

## Grammar of Tiny PL/0 (EBNF Format)

```  
program ::= block "." .
block ::= const-declaration var-declaration statement.
const-declaration ::= [ “const” ident "=" number {"," ident "=" number} “;"].
var-declaration ::= [ "var" ident {"," ident} “;"].
statement ::= [ ident ":=" expression
              | "begin" statement { ";" statement } "end"
              | "if" condition "then" statement 
              | "while" condition "do" statement
              | "read" ident 
              | "write" expression 
              | empty ] .  
condition ::= "odd" expression 
            | expression rel-op expression.  
rel-op ::= "="|“<>”|"<"|"<="|">"|">=“.
expression ::= [ "+"|"-"] term { ("+"|"-") term}.
term ::= factor {("*"|"/") factor}. 
factor ::= ident | number | "(" expression ")“.
```

## Error Messages
- Program must end with a period.
- 'const', 'var', 'read' keywords must be followed by an identifier.
- Duplicate symbol declarations are not allowed.
- Constants must be assigned with '=' and an integer value.
- Declarations must end with a semicolon.
- Only variable values may be altered using ':='.
- 'begin' must be followed by 'end'; 'if' by 'then'; 'while' by 'do'.
- Conditions must include a comparison operator.
- Expressions must contain valid operands and operators.

## Running the Code

To compile and run the parser and code generator, use the following commands:
```
gcc parser.c -o parser
./parser inputfile.txt
```

## Example Output

### Given Input:
```
const k = 8, c = 7;
var x, y;
begin
    x := k + y;
    if x > 10 then
        x := x - 10;
end.
```
### Output(Assembly Code):
```
Line    OP    L    M
  0    JMP    0    3
  1    INC    0    5
  2    LOD    0    3
  3    LOD    0    4
  4    OPR    0    2
  5    LIT    0    7
  6    OPR    0    8
  7    JPC    0    39
  8    LIT    0    8
  9    LOD    0    4
 10    OPR    0    1
 11    STO    0    3
 12    JMP    0    6
 13    LOD    0    3
 14    LIT    0    10
 15    OPR    0    2
 16    LIT    0    7
 17    OPR    0    6
 18    JPC    0    69
 19    LIT    0    7
 20    LOD    0    3
 21    OPR    0    1
 22    STO    0    3
 23    LOD    0    3
 24    LIT    0    3
 25    OPR    0    3
 26    LOD    0    4
 27    LIT    0    6
 28    OPR    0    2
 29    LOD    0    3
 30    OPR    0    4
 31    OPR    0    2
 32    OPR    0    11
 33    JPC    0    108
 34    LIT    0    7
 35    STO    0    4
 36    SYS    0    2
 37    STO    0    4
 38    LOD    0    3
 39    LIT    0    7
 40    OPR    0    1
 41    SYS    0    1
 42    SYS    0    3

Symbol Table:

Kind | Name        | Value | Level | Address | Mark
---------------------------------------------------
   1 |           k |     8 |     - |     - |     1
   1 |           c |     7 |     - |     - |     1
   2 |           x |     0 |     0 |     3 |     1
   2 |           y |     0 |     0 |     4 |     1
```

## Error Handling Example

### Input:
```
var x;
begin
    x := 10 + ;
end.
```

### Output:
```
Error: Arithmetic equations must contain operands, parentheses, numbers, or symbols.
```

**When executed successfully, the code also generates an `output.txt` file, containing the generated assembly code, which serves as input for HW1 (Virtual Machine).**
