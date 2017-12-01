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
    //Retorna el opcode actual
    unsigned short get_pc();

    //Returns a copy of the memory
    void get_mem(char* receive);

    //Pixel vector
    unsigned char gfx[64 * 32];

    //Initialize everything for use
    void init();

    bool drawFlag;

    //Records key input
    //Load game into memory
    void loadGame(string name);

    //Emulates 1 cyle
    void emulateCycle();

    //Flag that dictates wether something should be drawn
    void setKey(unsigned char addr, char state);

    unsigned char* getKeys();

};


#endif //INC_8CPINTERPRETE_BASE8CHIP_H
