# Compiler

## Authors: Ethan ,Faramarz Aboutalebi

## This code:
- reads pl0 code from text file
- generates Lexeme Table and token list on terminal
- run from command line by:
         gcc lex.c -o lex
          /.lex inputfile.txt


# Questions to ask from the TA:

1. Are we allowed to use the following error messages, regarding the fact that only three error messages were mentioned in HW2 instruction:
- Error : Identifiers cannot begin with a digit
- Error : Identifiers cannot exceed 11 characters in length
- Error : Numbers cannot exceed 5 digits
- Error : Invalid Symbol
- Error : Invalid Symbol
- Error : Invalid Symbol
- Error : Invalid Symbol
- Error : Invalid Symbol
- Error : Invalid Symbol


2. Do we need to display an invalid lexeme when we are displaying an error message for it?

3. Is the PL0 space sensitive? In other words, is "<   >" equal to "<>" or because of using white space "<" and ">" would consider different symbols. Or do "/*my comment*/" and "/  *  my comment  */" do the same thing.


