# C8OA (CHIP-8 Once Again)
C8OA is yet another CHIP-8 ~~emulator~~ interpreter written in C++ using SDL2. Made for fun and learning purposes, to potentially be used as a reference for future projects.

It is a work-in-progress, and as such, is not unstable and probably won't run most games/programs correctly.

## Building
You will need to have GCC and SDL2 installed on your system. You can install both using your package manager, or using MSYS2 on Windows.

Once you have them installed, you can build the project by simply running `make` in the root directory of the project. This will create an executable called `c8oa` in the `build` directory.

To delete the build directory, invoke `make clean`.

## Usage
To run a CHIP-8 program, you can just run the executable with the path to the ROM file as an argument. Here's an example:
```sh
./build/c8oa /path/to/rom.ch8
```

## Tests
Here is a list of tests from the [CHIP-8 test suite](https://github.com/Timendus/chip8-test-suite) and their working status:
- ✅ **CHIP-8 splash screen** ([`1-chip8-logo.ch8`](roms/1-chip8-logo.ch8))
- ✅ **IBM logo** ([`2-ibm-logo.ch8`](roms/2-ibm-logo.ch8))
- ✅ **Corax+ opcode test** ([`3-corax.ch8`](roms/3-corax.ch8))
- ✅ **Flags test** ([`4-flags.ch8`](roms/4-flags.ch8))
- ❌ **Quirks test** ([`5-quirks.ch8`](roms/5-quirks.ch8)) - after choosing the platform, interpreter exits out with a SEGFAULT
- ❌ **Keypad test** ([`6-keypad.ch8`](roms/6-keypad.ch8)) - not correctly showing the key presses
- ❌ **Beep test** ([`7-beep.ch8`](roms/7-beep.ch8)) - sound not implemented
- ❌ **Scrolling test** ([`8-scrolling.ch8`](roms/8-scrolling.ch8)) - scrolling not implemented

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.