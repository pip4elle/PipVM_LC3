# PipVM_LC3: an LC-3 virtual machine in C

A virtual machine for the **LC-3** (*Little Computer 3*), the 16-bit architecture
used in countless university courses to teach how a computer works at a low level.
The project reads an already-assembled LC-3 program (a binary `.obj` file), loads it
into memory and runs it, one instruction at a time, right in your terminal.

It's written in plain C with no external dependencies, with the goal of really
understanding what happens inside a CPU: fetch, decode, execute, condition flags,
memory-mapped I/O and traps.

---

## What is the LC-3, in short

- **16-bit** words and 16-bit addressing → **65,536 memory locations** (2^16 words)
- **8 general-purpose registers** (`R0`–`R7`), plus `PC` and a condition-flag register
- **16 opcodes** (14 of which are actually usable)
- Condition flags: **N**egative, **Z**ero, **P**ositive
- I/O through **trap routines** (`GETC`, `OUT`, `PUTS`, `IN`, `PUTSP`, `HALT`)

---

## Features

- All the core LC-3 instructions implemented:
  `BR`, `ADD`, `LD`, `ST`, `JSR/JSRR`, `AND`, `LDR`, `STR`, `NOT`, `LDI`, `STI`, `JMP`, `LEA`, `TRAP`
- Six trap codes handled: `GETC` (0x20), `OUT` (0x21), `PUTS` (0x22), `IN` (0x23), `PUTSP` (0x24), `HALT` (0x25)
- `.obj` loading with **endianness** handling (LC-3 files are big endian, your PC almost certainly isn't)
- Check that the program fits in the user area (`0x3000`–`0xFDFF`)
- **Non-blocking keyboard input** through the memory-mapped `KBSR`/`KBDR` registers (`0xFE00` / `0xFE02`)
- Terminal switched to raw mode while running and **always restored** on exit, even on `Ctrl+C`
- Code split into small, readable modules (CPU, opcodes, traps, memory, input, init)
- A single `Makefile`, no libraries to install

---

## How it works

The heart of the VM is the classic CPU loop, found in `main.c`:

```
while (running) {
    fetch    →  read the instruction at address PC and increment PC
    execute  →  decode the opcode and call the matching function
}
```

### Project structure

| File | What it does |
|------|--------------|
| `main.c` | Entry point: initializes, loads the program, runs the fetch/execute loop, cleans up |
| `cpu.c / cpu.h` | `fetch`, `execute`, `setcc`, plus the register and flag definitions |
| `opcodes.c / opcodes.h` | One implementation per instruction, plus macros for bit extraction and sign/zero extension |
| `trapcodes.c / trapcodes.h` | The trap routines (I/O and halt), implemented directly in C |
| `memory.h` | RAM size, memory regions and addresses of the memory-mapped registers |
| `input.c / input.h` | Terminal handling and `mem_read` with the keyboard logic |
| `init.c / init.h` | CPU/RAM allocation and `.obj` file loading |
| `usage.c / usage.h` | Command-line help message |

### A few interesting details

- **Dispatch via a function-pointer table**: the top 4 bits of the instruction
  (`instr >> 12`) index into an array, so there's no giant `switch`.
- **Bit macros**: `MID_BITS`, `SEXT16` and `ZEXT16` extract fields from the
  instruction and handle sign extension, which is the trickiest part of an LC-3 VM.
- **Condition flags**: after every instruction that writes a register, `setcc`
  updates the `PSR` register with one of `POS`, `ZERO`, `NEG`; `BR` reads them to decide whether to jump.
- **Memory-mapped I/O**: when `KBSR` is read, the VM uses `select()` to check whether a
  key is available; if so, it puts the character in `KBDR` and sets bit 15 of `KBSR`.
- **Traps**: every `TRAP` saves `PC` into `R7` and then runs the matching routine.
  The routines aren't written in LC-3 assembly but emulated directly in C.

---

## Building

All you need is `gcc`. The `Makefile` expects this layout:

```
.
├── Makefile
├── src/
│   ├── *.c
│   └── inc/        ← the headers (*.h)
└── out/
    └── bin/        ← the executable ends up here
```

Then:

```bash
mkdir -p out/bin
make
```

This produces `./out/bin/PipVM_LC3`.

## Usage

```bash
./out/bin/PipVM_LC3 program.obj
```

The program must be an already-assembled LC-3 object file (i built them with the
`lc3as` assembler). The first word of the file is the origin address; the rest is the code.
`Ctrl+C` stops execution and restores the terminal to normal.

---

## Pros

- **Easy to read**: few files, short functions, clear names. Great for studying how a CPU works.
- **Zero dependencies**: just the C standard library and POSIX headers (no need to know them).
- **Fast**: no interpreter or framework overhead; dispatch is a plain call through a table.
- **Clean terminal handling**: no "broken" console after a crash or `Ctrl+C`.
- **Easy to extend**: adding a trap or changing an instruction means touching a single spot.
- **A good testbed** for hand-written LC-3 assembly programs or course exercises.

## Cons and limitations

The project is deliberately minimal, so:

- **POSIX only** (Linux, macOS): it uses `termios` and `select`, so it doesn't build on native Windows (WSL works).
- **`RTI` and `RES` are not implemented**: if the program executes them, the VM stops with an error.
- **No interrupts, no supervisor/user modes, no system stack**: the `PSR` register only holds the condition flags.
- **Traps are emulated in C**, not loaded as real system routines in memory: convenient, but less faithful to real hardware.
- **Only the keyboard is memory-mapped**: the display registers (`DSR`/`DDR`) are missing; output goes straight through traps.
- **No debugger, trace or single-step**: to see what a program is doing you have to add prints by hand.
- **No automated tests** for now: verification is done by running assembly test programs.
- **`.obj` validation could be better**: for instance, load bounds are checked after the data has been read into memory.

---

## Future ideas

- Implement `RTI`, interrupts and privileged mode
- Add the `DSR`/`DDR` registers for the display
- Debug mode (single-step, register and memory dumps)
- Test suite with assembly programs and expected output
- Windows support

---

## References

- The LC-3 ISA specification (Patt & Patel, *Introduction to Computing Systems*)
- The best way to try it: write a small LC-3 assembly program, assemble it with `lc3as` and run it on this VM
- [The LC-3 ISA, Appendix A](https://highered.mheducation.com/sites/dl/free/0072467509/104691/pat67509_appa.pdf)
- [Write your Own Virtual Machine By: Justin Meiners and Ryan Pendleton](https://www.jmeiners.com/lc3-vm/)