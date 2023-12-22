# README: HW4 - Extended Recursive Descent Parser for PL/0

## Objective
This assignment, HW4, extends the functionality of HW3 to incorporate additional grammatical constructs, particularly `procedure` and `call`, in the PL/0 language. The aim is to parse PL/0 programs that include these new constructs and generate intermediate code.

## Grammar of PL/0

### Extended Backus-Naur Form (EBNF) of PL/0

```ebnf
program ::= block "." .
block ::= const-declaration var-declaration procedure-declaration statement.
const-declaration ::= ["const" ident "=" number {"," ident "=" number} ";"].
var-declaration ::= [ "var" ident {"," ident} “;"].
procedure-declaration ::= { "procedure" ident ";" block ";" }
statement ::= [ ident ":=" expression
              | "call" ident
              | "begin" statement { ";" statement } "end"
              | "if" condition "then" statement
              | "while" condition "do" statement
              | "read" ident
              | "write" expression
              | empty ] .  
condition ::= "odd" expression | expression rel-op expression.  
rel-op ::= "=" | “<>” | "<" | "<=" | ">" | ">=“.
expression ::= term { ("+" | "-") term}.
term ::= factor { ("*" | "/") factor}. 
factor ::= ident | number | "(" expression ")“.
number ::= digit {digit}.
ident ::= letter {letter | digit}.
digit ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9“.
letter ::= "a" | "b" | … | "y" | "z" | "A" | "B" | ... | "Y" | "Z".
```

## Execution Instructions

To compile and run the PL/0 compiler, execute the following commands:

```
gcc hw4compiler.c -o hw4compiler
./hw4compiler inputfile.txt
```

## Example Usage

### Input PL/0 Program:
```
var x,y,z,v,w;
procedure a;
var x,y,u,v;
procedure b;
var y,z,v;
procedure c;
var y,z;
begin
z:=1;
x:=y+z+w
end;
begin
y:=x+u+w;
call c
end;
begin
z:=2;
u:=z+w;
call b
end;
begin
x:=1; y:=2; z:=3; v:=4; w:=5;
x:=v+w;
write z;
call a;
end.
```

## Expected Output
```
Source Program:
var x,y,z,v,w;
procedure a;
var x,y,u,v;
procedure b;
var y,z,v;
procedure c;
var y,z;
begin
z:=1;
x:=y+z+w
end;
begin
y:=x+u+w;
call c
end;
begin
z:=2;
u:=z+w;
call b
end;
begin
x:=1; y:=2; z:=3; v:=4; w:=5;
x:=v+w;
write z;
call a;
end.


No errors, program is syntactically correct

Line    OP    L    M
  0    JMP    0    96
  1    JMP    0    69
  2    JMP    0    42
  3    JMP    0    12
  4    INC    0    5
  5    LIT    0    1
  6    STO    0    4
  7    LOD    0    3
  8    LOD    0    4
  9    OPR    0    1
 10    LOD    3    7
 11    OPR    0    1
 12    STO    2    3
 13    OPR    0    0
 14    INC    0    6
 15    LOD    1    3
 16    LOD    1    5
 17    OPR    0    1
 18    LOD    2    7
 19    OPR    0    1
 20    STO    0    3
 21    CAL    0    9
 22    OPR    0    0
 23    INC    0    7
 24    LIT    0    2
 25    STO    1    5
 26    LOD    1    5
 27    LOD    1    7
 28    OPR    0    1
 29    STO    0    5
 30    CAL    0    6
 31    OPR    0    0
 32    INC    0    8
 33    LIT    0    1
 34    STO    0    3
 35    LIT    0    2
 36    STO    0    4
 37    LIT    0    3
 38    STO    0    5
 39    LIT    0    4
 40    STO    0    6
 41    LIT    0    5
 42    STO    0    7
 43    LOD    0    6
 44    LOD    0    7
 45    OPR    0    1
 46    STO    0    3
 47    LOD    0    5
 48    SYS    0    1
 49    CAL    0    3
 50    SYS    0    3
```

## Error Handling Example

### Input with Error:
```
/* Error: Symbol name has already been declared. */

const B = 12;
var u,h,i,g,p, f, n;
procedure fun1;
  var x,y,z ,n, m;
  var x;
  procedure fun2;
    var u,   p;
    procedure fun3;
      var x,y  , z;
      begin
        z:=h*p+i;
        while g+u <> z do
          call fun1;
        call fun3
      end;
    begin
      z := x+y;

    end;
  begin
    if odd u+p +m then
      call fun1;
  end;

procedure sum;
  var q, k;
  begin
    if q/k <> B then
      call sum
  end;



begin
  read p;
  write u / h;
  call fun1;
  call sum

end.
```

### Error Output:
```
Error: Symbol name has already been declared.
```

**Upon successful execution, the compiler generates an `elf.txt` file containing the assembly code, which serves as input for HW1 (Virtual Machine).**
