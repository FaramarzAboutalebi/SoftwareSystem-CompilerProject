# README for PL/0 Lexical Analyzer

## Overview
This tool is a lexical analyzer for the PL/0 programming language. It takes a source program written in PL/0, identifies certain errors, and generates three key outputs: the source program, a lexeme table, and a token list.

## Context-Free Grammar for PL/0 (EBNF Format)

### Program Structure
- `program ::= block "." .`
- `block ::= const-declaration var-declaration proc-declaration statement.`

### Declarations
- `const-declaration ::= [ “const” ident "=" number {"," ident "=" number} “;"].`
- `var-declaration ::= [ "var" ident {"," ident} “;"].`
- `proc-declaration ::= {"procedure" ident ";" block ";" } .`

### Statements
- `statement ::= [ ident ":=" expression | "call" ident | "begin" statement { ";" statement } "end" | "if" condition "then" statement | "ifel" condition "then" statement “else" statement | "while" condition "do" statement | “read” ident | “write” ident | empty ] .`
- `empty ::= `

### Expressions and Conditions
- `condition ::= expression rel-op expression.`
- `rel-op ::= "="|“<>"|"<"|"<="|">"|">=“.`
- `expression ::= [ "+"|"-"] term { ("+"|"-") term}.`
- `term ::= factor {("*"|"/") factor}. `
- `factor ::= ident | number | "(" expression ")“.`

## Lexical Grammar (EBNF Format)

- `ident ::= letter {letter | digit}.`
- `letter ::= "a" | "b" | … | "y" | "z" | "A" | "B" | ... | "Y" | "Z".`
- `number ::= digit {digit}.`
- `digit ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9“.`

## Lexical Conventions

Numerical values are assigned to each token as follows: 
- `skipsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6, slashsym = 7, ifelsym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33.`

## Running the Analyzer

Compile and execute the analyzer using the following commands:
```bash
gcc lex.c -o lex
./lex input
```

### Example

- Input:

```bash
var x, y;
begin
	x := y * 2;
end.
```


- The output:

```bash
Source Program:
var x, y;
begin
	x := y * 2;
end.

Lexeme Table:
lexeme		token type
var         29
x           2
,           17
y           2
;           18
begin       21
x           2
:=          20
y           2
*           6
2           3
;           18
end         22
.           19
Token List:
29 2 x 17 2 y 18 21 2 x 20 2 y 6 3 2 18 22 19 
```
