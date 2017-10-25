//
// Created by Martin on 21/10/2017.
//

#include <string>

#ifndef INC_8CPINTERPRETE_BASE8CHIP_H
#define INC_8CPINTERPRETE_BASE8CHIP_H


using namespace std;


class Base8Chip {
private:
    //Current instruction
    unsigned short opcode;

    //Memory of the emulator
    unsigned char memory[4096];

    //Registry
    unsigned char V[16];

    //Indexes
    unsigned short I;
    unsigned short pc;

    //Timers
    unsigned char delay_timer;
    unsigned char sound_timer;

    //Stack
    unsigned short stack[16];

    //Stack pointer
    unsigned short sp;

    //State of keys
    unsigned char key[16];

public:
    //Pixel vector
    unsigned char gfx[64 * 32];

    //Initialize everything for use
    void init();

    //Load game into memory
    void loadGame(string name);

    //Emulates 1 cyle (60 cycles per second)
    void emulateCycle();

    //Flag that dictates wether something should be drawn
    bool drawFlag;

    //Records key input
    void setKey(const unsigned char addr, const char state);

    //Fonts map
    unsigned char const chip8_fontset[80] =
            {
                    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };
};


#endif //INC_8CPINTERPRETE_BASE8CHIP_H
