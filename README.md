# Y86-Simulater
## Usage

***For Sequential***

```bash
cd sequential
make
./y86-simulater < test/example.yo > example.json 
```

or:

```bash
cd sequential
make
python3 test.py --bin build/backend
```

***For Pipeline***

```bash
cd pipelined
cmake .
make
./y86-simulater < test/example.yo > example.json 
```

***For frontend***

- Open with QT creator
- set test file path `widget.cpp: 13`
- build and run
- click start
- click next to step

## Sequential

![arch](<./images/seq_arch.png>)

### memory

***readin.cpp***

To read and parse hexadecimal data from an input file, storing this data into the memory map.

Firstly, it parse each line of the input file according to the position of the `colon` and `delimiter`. Then, it converts the hexadecimal string into a binary string and stores it into the memory map.

We use `address` as `key` and a special data structur containing vector `data` and vector `rdata` as `value` to store the data,  handling the endianess by storing bytes in reverse order in the data array. The benefit of this design is that we can easily access the data in the memory map by address and check validity of the address.

***read and readByte***

It computes the block address and the offset, checks the validity of the address, and then constructs the 64-bit value or byte from the memory blocks.

When we `readByte`, the address is not necessarily aligned, so we need to read the data in proper block and the next block, and then `merge` them together, `extracting` the block we need.

***write and writeByte***

This funtion validates the address alignment and writes the 64-bit value or byte value into the memory map, handling the endianess by storing bytes in reverse order in the data array.

![ins](<./images/ins.png>)

### fetch

***Single Byte Instructions (halt, nop, ret):***

If the instruction is `halt`, `nop`, or `ret`, the function increments the PC by 1 (this->PC + 1), indicating that the instruction size is 1 byte.

***Two Byte Instructions (rrmovq, opq, pushq, popq):***

If the instruction is `rrmovq`, `opq`, `pushq`, and `popq`, an additional byte is read from memory. The PC is incremented by 2 to move to the next instruction.

***Ten Byte Instructions (irmovq, rmmovq, mrmovq):***

These instructions involve reading another byte for register information and then a 64-bit value (`valC`), which is read from memory starting from the address PC + 2.

***Nine Byte Instructions (jxx, call):***

For `jxx` (jump) and call instructions, a 64-bit value (`valC`) is read from memory starting from PC + 1.

***Default Case (Invalid Instruction):***

If the instruction code doesn't match any of the known instructions, the function sets a status (`STAT_INS`) indicating an invalid instruction.

### decode

***No Operation for Certain Instructions (halt, nop, irmovq, jxx):***

No decoding is needed.

***General Register-to-Register and Memory Operations (rrmovq, rmmovq, mrmovq, opq):***

Involve two registers. The values of these registers (`rA` and `rB`) are fetched from the register file (`RF`) and stored in `valA` and `valB` respectively.

***Stack Operations (call, ret, popq):***

These instructions involve stack manipulations. The stack pointer register (`rsp`) is used to get values for both `valA` and `valB`.

***Stack Push Operation (`pushq`):***

For the `pushq` instruction, the value of the register specified in `rA` is fetched into `valA`, and the stack pointer register's value (`rsp`) is fetched into valB, which aligns with the push operation's behavior, where a value is pushed onto the stack pointed by `rsp`.

***Default Case (Invalid Instruction):***

Sets a status (`STAT_INS`).

### execute

***Halt (halt):***

Sets the CPU status to STAT_HLT.

***No Operation (nop):***

Performs no action.

***Register-to-Register Move or Conditional Jump (rrmovq, jxx):***

Assigns `valA` to `valE`. For `jxx`, it also sets the condition flag with `set_Cnd()`. This is consistent with move operations and preparing for conditional jumps.

***Immediate to Register Move (irmovq):***

Directly assigns the immediate value (`valC`) to `valE`.

***Memory Move Operations (rmmovq, mrmovq):***

Calculates `valE` as the sum of `valB` and `valC`.

***Arithmetic and Logical Operations (opq):***

Performs a specified operation (like `add`, `subtract`, etc.) and gets the result in `valE`. It also sets condition codes with `set_CC` as needed.

***Call and Push to Stack (call, pushq):***

Decrease the stack pointer (stored in `valB`) by 8 to make space on the stack.

***Return from Call and Pop from Stack (ret, popq):***

Increase the stack pointer by 8, removing the top value from the stack or returning from a call.

***Default Case (Invalid Instruction):***

Sets the status (`STAT_INS`) to indicate an invalid instruction.

### PC Update

***Halt (halt):***

No action is taken.

***Simple Instructions (nop, rrmovq, irmovq, rmmovq, mrmovq, opq, pushq, popq):***

For these instructions, the `PC` is updated to `valP`.

***Conditional Jump (jxx):***

Updates the `PC` based on the condition flag (`Cnd`). If the condition is true, PC is set to `valC` (usually a jump target address); otherwise, it's set to `valP` (the next sequential instruction).

***Function Call (call):***

Sets the `PC` to `valC`.

***Function Return (ret):***

Sets the `PC` to `valM`, which is the return `address`.

### writeBack

***No Operation (halt, nop, rmmovq, jxx):***

No write-back action is required.

***Register-to-Register Move (rrmovq):***

Writes `valE` to register `rB` if the condition (`Cnd`) is true; otherwise, it writes the original value of `rB` (`valB`) back to `rB`.

***Immediate to Register Move (irmovq), Arithmetic and Logical Operations (opq):***

Writes the result stored in `valE` directly to register `rB`.

***Memory to Register Move (mrmovq):***

Writes the memory content (`valM`) to register `rA`. This is typical for load operations from memory into a register.

***Stack Operations (call, ret, pushq):***

Updates the stack pointer register (`rsp`) with the new value (`valE`).

***Stack Pop Operation (popq):***

Similar to other stack operations, updates the stack pointer register (`rsp`). Additionally, writes the memory content (`valM`) to register `rA`.

## Pipelined

### CPU Architecture

![pipeline architecture](<./images/pipe_arch.png>)

#### Note

1. Uppercase letters represent ***registers*** of the stage, such as `E_dstE` representing the register before the `execute` stage

2. Lowercase letters represent ***output ports***. This is because we need to simulate the values passed between components on the physical layer in a serial program. For example, `e_dstE` represents the output value after the `execute` stage. This value is not directly accessed but is passed on to other hardware components.

### Clock Cycle

```cpp
void CPU::run() {
    while (true) {
        fetch();
        decode();
        execute();
        memory_access();
        write_back();
        forward();
        control();
        pass_params();
        if (this->STAT != STAT_AOK) break;
        this->record_stage();
    }
    this->record_stage();
}
```

The functions `fetch`, `decode`, `execute`, `memory_access`, and `write_back` simulate the serialization of five components within the same clock cycle. The order is variable and does not affect the output.

### Fetch

In pipeline design, the control of the `program_counter` is also implemented in the `fetch` stage, with the specific sequence as follows:

1. `Select_PC`: Choose the current round's `PC` from three sources.
2. `fetch`: Fetch an instruction from memory.
3. `Predict_PC`: Predict the next round's `PC`, and the output is `f_PredPC` (actually a wire connection, not a physical register, denoted in lowercase `f`). If `F_stall==false`, the value of `f_PredPC` is written to `F_PredPC`; otherwise, it is not, achieving the effect of a `stall`.

### Decode

Similar to the `sequential` implementation, values are primarily stored in registers or in output ports.

### Execute

According to the design in the book, this stage is split into three main sub-stages: `ALU_A`, `ALU_B`, and `ALU`. Based on the corresponding value, `E_set_CC` sets the corresponding `conditional codes`. It is worth noting that when `m_stat != STAT_AOK || W_stat != STAT_AOK`, `E_set_CC` is disabled (this is because an erroneous instruction has been executed, and the change to visible `conditional codes` should be immediately disabled to prevent unexpected behavior in the processor). After `E_set_CC`, `E_set_Cnd` determines the value of `e_Cnd` based on `conditional codes` and `E_ifun`. This decision determines whether `jmpXX` and `cmov` instructions are executed.

### forward

`forward` implements forwarding, simulating the `Fwd_B` and `Sel+Fwd_A` components. It determines whether to forward based on the values in the required registers and completes the forwarding operation.

### control

`control` handles four cases:

1. Processing `ret`
2. Load/use hazard
3. Mispredicted branches
4. Exceptions (halt, invalid memory access, invalid instructions)

It detects the corresponding conditions and sets the appropriate `bubble` or `stall` registers.

### pass_params

`pass_params` simulates the part of the clock rising edge where register values change. The value transfer includes both from register to register and from output values to registers. The setting of `stall` and `bubble` takes effect here, determining whether certain values should be transferred based on the corresponding conditions.

## frontend

Modify the io part of the original code, and add the following functions:

On clicking the `start` button, the program will read the file and initialize the memory. Then, it will start the simulation continuously till the end of the program.

Additionally, we store the temporary values of the registers and instructions such as `icode` and `ifun` in each clock cycle. Finally, we will get total vector containing all stages of the program.

On clicking the `next` button, the program performs one clock cycle of simulation and updates the values of the registers and instructions, which is similar to the `si` command in `gdb`.

For clarity, we use names of the operations and registers instead of the corresponding values.

![frontend](<./images/demo.png>)
