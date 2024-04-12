#!/bin/sh
set -xeuo pipefail
gcc ./assembler/compiler.c -o ./compiler.out 
gcc ./assembler/vm.c -o ./vm.out
gcc ./parser/parser.c -o ./parser.out

./parser.out neander.math -o ./neander.asm
./compiler.out neander.asm -o ./neander.mem
./vm.out neander.mem

rm ./vm.out
rm ./compiler.out
rm ./parser.out
