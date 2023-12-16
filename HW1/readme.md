# HW1: P-Machine Implementation

## Introduction
This project features a P-machine (stack machine) with a single memory area known as the process address space (PAS). The PAS comprises two segments: the "text", containing executable VM instructions, and the "stack", utilized by the PM/0 CPU as a data stack.

The provided source code (`vm.c`) processes assembly code and outputs the stack's state for each input assembly line, alongside the result. The assembly for this project adheres to a specific Instruction Set Architecture (ISA).

## Instruction Set Architecture (ISA)

### Overview
The ISA includes 10 arithmetic/logical operations for data manipulation within the stack segment. These operations are activated by setting the OP component to 2 (OPR). The M component of the OPR instruction determines the specific operation (e.g., to multiply the top two stack elements, use “2 0 4”).

### Instructions

- `01 - LIT 0, M`: Push constant value M onto the stack.
- `02 - OPR 0, M`: Perform an operation on top stack data or function return.
- `03 - LOD L, M`: Load top of stack from an offset M in Activation Record (AR) at L levels down.
- `04 - STO L, M`: Store top of stack into an offset M in AR at L levels down.
- `05 - CAL L, M`: Call procedure at code index M, creating a new Activation Record.
- `06 - INC 0, M`: Allocate M memory words, incrementing SP by M.
- `07 - JMP 0, M`: Jump to instruction M.
- `08 - JPC 0, M`: Conditional jump based on top stack element.
- `09 - SYS 0, M`: System calls for output, input, and program termination.

### ISA Pseudo Code

- `01 - LIT 0, M`: Stack operations for pushing literals.
- `02 - OPR 0, #`: Range of arithmetic and logical operations.
- `03 - LOD L, M`: Load operation.
- `04 - STO L, M`: Store operation.
- `05 - CAL L, M`: Procedure call handling.
- ... (and so on for remaining instructions)

## Running the Code

To compile and run the code, use the following commands:

```bash
gcc vm.c -o vm
./vm [name of input file]

```

### Example

- recursive_input.txt:
```bash
7 0 75
7 0 6
6 0 4
3 1 4
4 0 3
3 1 4
1 0 1
2 0 2
4 1 4
3 1 4
1 0 0
2 0 5
8 0 45
1 0 1
4 1 3
3 1 4
1 0 0
2 0 9
8 0 60
5 1 6
3 1 3
3 0 3
2 0 3
4 1 3
2 0 0
6 0 5
1 0 3
4 0 4
5 0 6
3 0 3
9 0 1
9 0 3
```

- The output would be:

 ```bash
                 PC  BP  SP  stack
Initial values:	 0	 96  95

    JMP  0   75  75  96  95  
    INC  0   5   78  96  100 0  0  0  0  0  
    LIT  0   3   81  96  101 0  0  0  0  0  3  
    STO  0   4   84  96  100 0  0  0  0  3  
    CAL  0   6   6   101 100 0  0  0  0  3  
    INC  0   4   9   101 104 0  0  0  0  3  |   96  96  87  0  
    LOD  1   4   12  101 105 0  0  0  0  3  |   96  96  87  0  3  
    STO  0   3   15  101 104 0  0  0  0  3  |   96  96  87  3  
    LOD  1   4   18  101 105 0  0  0  0  3  |   96  96  87  3  3  
    LIT  0   1   21  101 106 0  0  0  0  3  |   96  96  87  3  3  1  
    SUB  0   2   24  101 105 0  0  0  0  3  |   96  96  87  3  2  
    STO  1   4   27  101 104 0  0  0  0  2  |   96  96  87  3  
    LOD  1   4   30  101 105 0  0  0  0  2  |   96  96  87  3  2  
    LIT  0   0   33  101 106 0  0  0  0  2  |   96  96  87  3  2  0  
    EQl  0   5   36  101 105 0  0  0  0  2  |   96  96  87  3  0  
    JPC  0   45  45  101 104 0  0  0  0  2  |   96  96  87  3  
    LOD  1   4   48  101 105 0  0  0  0  2  |   96  96  87  3  2  
    LIT  0   0   51  101 106 0  0  0  0  2  |   96  96  87  3  2  0  
    GTR  0   9   54  101 105 0  0  0  0  2  |   96  96  87  3  1  
    JPC  0   60  57  101 104 0  0  0  0  2  |   96  96  87  3  
    CAL  1   6   6   105 104 0  0  0  0  2  |   96  96  87  3  
    INC  0   4   9   105 108 0  0  0  0  2  |   96  96  87  3  |   96  101  60  0  
    LOD  1   4   12  105 109 0  0  0  0  2  |   96  96  87  3  |   96  101  60  0  2  
    STO  0   3   15  105 108 0  0  0  0  2  |   96  96  87  3  |   96  101  60  2  
    LOD  1   4   18  105 109 0  0  0  0  2  |   96  96  87  3  |   96  101  60  2  2  
    LIT  0   1   21  105 110 0  0  0  0  2  |   96  96  87  3  |   96  101  60  2  2  1  
    SUB  0   2   24  105 109 0  0  0  0  2  |   96  96  87  3  |   96  101  60  2  1  
    STO  1   4   27  105 108 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  
    LOD  1   4   30  105 109 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  1  
    LIT  0   0   33  105 110 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  1  0  
    EQl  0   5   36  105 109 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  0  
    JPC  0   45  45  105 108 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  
    LOD  1   4   48  105 109 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  1  
    LIT  0   0   51  105 110 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  1  0  
    GTR  0   9   54  105 109 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  1  
    JPC  0   60  57  105 108 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  
    CAL  1   6   6   109 108 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  
    INC  0   4   9   109 112 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  |   96  105  60  0  
    LOD  1   4   12  109 113 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  |   96  105  60  0  1  
    STO  0   3   15  109 112 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  
    LOD  1   4   18  109 113 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  1  
    LIT  0   1   21  109 114 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  1  1  
    SUB  0   2   24  109 113 0  0  0  0  1  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  0  
    STO  1   4   27  109 112 0  0  0  0  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  
    LOD  1   4   30  109 113 0  0  0  0  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  0  
    LIT  0   0   33  109 114 0  0  0  0  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  0  0  
    EQl  0   5   36  109 113 0  0  0  0  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  1  
    JPC  0   45  39  109 112 0  0  0  0  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  
    LIT  0   1   42  109 113 0  0  0  0  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  1  
    STO  1   3   45  109 112 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  
    LOD  1   4   48  109 113 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  0  
    LIT  0   0   51  109 114 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  0  0  
    GTR  0   9   54  109 113 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  0  
    JPC  0   60  60  109 112 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  
    LOD  1   3   63  109 113 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  1  
    LOD  0   3   66  109 114 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  1  1  
    MUL  0   3   69  109 113 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  1  
    STO  1   3   72  109 112 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  |   96  105  60  1  
    RTN  0   0   60  105 108 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  
    LOD  1   3   63  105 109 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  1  
    LOD  0   3   66  105 110 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  1  2  
    MUL  0   3   69  105 109 0  0  0  1  0  |   96  96  87  3  |   96  101  60  2  2  
    STO  1   3   72  105 108 0  0  0  2  0  |   96  96  87  3  |   96  101  60  2  
    RTN  0   0   60  101 104 0  0  0  2  0  |   96  96  87  3  
    LOD  1   3   63  101 105 0  0  0  2  0  |   96  96  87  3  2  
    LOD  0   3   66  101 106 0  0  0  2  0  |   96  96  87  3  2  3  
    MUL  0   3   69  101 105 0  0  0  2  0  |   96  96  87  3  6  
    STO  1   3   72  101 104 0  0  0  6  0  |   96  96  87  3  
    RTN  0   0   87  96  100 0  0  0  6  0  
    LOD  0   3   90  96  101 0  0  0  6  0  6  
The output result is: 6
    SYS  0   1   93  96  100 0  0  0  6  0  
    SYS  0   3   96  96  100 0  0  0  6  0
```

### `The output demonstrates the machine's PC, BP, SP, and stack status throughout the execution of the given input assembly.`
