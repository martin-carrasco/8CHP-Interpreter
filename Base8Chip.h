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
    
    //Extended mode enabled
    bool is_extended;

public:
    //Key press checker
    bool key_pressed = false;
    
    //Pixel vector
    unsigned char gfx[64 * 32];
    
    //Pixel vector for extended mode
    unsigned char gfx_e[128 * 64];
    
    //Initialize everything for use
    void init();

    bool drawFlag;

    //Records key input
    //Load game into memory
    void loadGame(string name);

    //Emulates 1 cyle (60 cycles per second)
    void emulateCycle();

    //Flag that dictates wether something should be drawn
    void setKey(unsigned char addr, char state);

    unsigned char* getKeys();

};


#endif //INC_8CPINTERPRETE_BASE8CHIP_H
