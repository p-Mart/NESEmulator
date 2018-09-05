WIP Attempt at making a cycle-accurate NES Emulator, in C++

## Installation Instructions
This project requires SDL2 for rendering:

#### Debian / Ubuntu
```sudo apt-get install libsdl2-dev```

## Build instructions

You can build this emulator, in its current state, by doing:
```make tests```

## Usage

Currently, the emulator will run in an infinite loop and output large amounts of text (for debugging purposes).
It's recommended to redirect the output to /dev/null, or something of the sort.

You can run the emulator by doing:
```./RunTests.out [romname].nes > /dev/null```

To close it, use CTRL+C and close the SDL window for the emulator.