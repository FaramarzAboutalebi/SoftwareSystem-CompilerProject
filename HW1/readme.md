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
