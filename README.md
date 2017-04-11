# cpu2 #
---

The spritual successor of my original CPU simulator (called cpu).

## Installation ##
Requires JCRL (only the stack module though).

    git clone https://github.com/jmcph4/jcrl.git
    cd jcrl
    make
    cd ..
    git clone https://github.com/jmcph4/cpu2.git
    cp jcrl/build/libjcrl.a cpu2/lib/libjcrl.a
    cp jcrl/src/* cpu2/include/
    cd cpu2
    make

## Usage ##
Accepts a file containing machine code, outputs CPUI's status to the screen for each instruction.

    cpu2 machine_code.bin

See wiki or docs (soon) for ISA details.

