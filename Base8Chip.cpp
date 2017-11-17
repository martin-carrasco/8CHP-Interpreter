//
// Created by Martin on 21/10/2017.
//

#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>
#include <assert.h>
#include "Base8Chip.h"

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

void Base8Chip::init() {

    //inits
    srand(time(NULL));
    drawFlag = true;
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    sound_timer = 0;
    delay_timer = 0;

    //Zeroes the memory
    fill(key, key+16, 0);
    fill(stack, stack+16, 0);
    fill(V, V+16, 0);
    fill(memory, memory+4096, 0);
    fill(gfx, gfx+(32*64), 0);

    //Load fonts
    for(int i = 0;i < 80;i++){
        memory[i] = chip8_fontset[i];
    }
}
void Base8Chip::emulateCycle() {

    //Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    //Decode opcode and exec
    switch (opcode & 0xF000)
    {
        //Clear CLS
        case 0x0000:
            switch(opcode & 0x000F)
            {
                case 0x0000:
                    fill(gfx, gfx+(32*64), 0);
                    drawFlag = true;
                    pc+=2;
                    break;
                case 0x000E:
                    --sp;
                    pc=stack[sp];
                    pc += 2;
                    break;
            }
            break;
        case 0x1000:
            pc = 0x0FFF & opcode;
            break;
        case 0x2000:
            stack[sp] = pc;
            ++sp;
            pc = 0x0FFF & opcode;
            break;
        case 0x3000:
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc+=4;
            else
                pc+=2;
            break;
        case 0x4000:
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                pc+=4;
            else
                pc+=2;
            break;
        case 0x5000:
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc+=4;
            else
                pc+=2;
            break;
        case 0x6000:
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc+=2;
            break;
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc+=2;
            break;
        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0000:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                    break;
                case 0x0001:
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                    break;
                case 0x0002:
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                    break;
                case 0x0003:
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                    break;
                case 0004:
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0005:
                    if(V[(opcode & 0x00F0) >> 8] > V[(opcode & 0x0F00) >> 8])
                        V[0xF] = 0;
                    else
                        V[0xF] = 1;
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                    break;
                case 0x0006: {
                    V[0xF] = 1 & V[(opcode & 0x0F00) >> 8];
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                    break;
                }
                case 0x0007: {
                    if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
                        V[0xF] = 0;
                    else
                        V[0xF] = 1;
                    V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8]);
                    pc += 2;
                    break;
                }
                case 0x000E:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc+=2;
                    break;
            }
            break;
        case 0x9000:
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
                pc+=4;
            else
                pc+=2;
            break;
        case 0xA000:
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xB000:
            pc = V[0] + (opcode & 0x0FFF);
            break;
        case 0xC000:
            V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
            pc+=2;
            break;
        case 0xD000: {
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short h = opcode & 0x000F;
            unsigned short pixel;

            V[0xF] = 0;
            for (int ypos = 0; ypos < h; ypos++) {
                pixel = memory[I + ypos];
                for (int xpos = 0; xpos < 8; xpos++) {
                    if ((pixel & (0x80 >> xpos)) != 0) {
                        if (gfx[(x + xpos + ((y + ypos) * 64))] == 1)
                            V[0xF] = 1;
                        gfx[x + xpos + ((y + ypos) * 64)] ^= 1;
                    }
                }
            }
            drawFlag = true;
            pc += 2;
            break;
        }
        case 0xE000:
            switch (opcode & 0x00FF)
            {
                case 0x009E:
                    if (key[V[(opcode & 0x0F00) >> 8]] == 1)
                        pc += 4;
                    else
                        pc += 2;
                case 0x00A1:
                    if (key[V[(opcode & 0x0F00) >> 8]] == 0)
                        pc += 4;
                    else
                        pc += 2;
                    break;
            }
            break;
        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007:
                    V[opcode & 0x0F00 >> 8] = delay_timer;
                    pc+=2;
                    break;
                case 0x000A:
                    for(int i = 0;i < 16;i++){
                        if(key[i] == 1) {
                            V[(opcode & 0x0F00) >> 8] =  i;
                            pc+=2;
                            break;
                        }
                    }
                    return;
                case 0x0015:
                    delay_timer = V[opcode & 0x0F00 >> 8];
                    pc+=2;
                    break;
                case 0x0018:
                    sound_timer = V[opcode & 0x0F00 >> 8];
                    pc+=2;
                    break;
                case 0x001E:
                    if(I + V[opcode & 0x0F00 >> 8 ] > 0xFFF)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    I += V[opcode & 0x0F00 >> 8];
                    pc+=2;
                    break;
                case 0x0029:
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc += 2;
                    break;
                case 0x0033:
                    memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
                    pc += 2;
                    break;
                case 0x0055:
                    for(int x = 0;x < ((opcode & 0x0F00) >> 8);x++){
                        memory[I+x] = V[x];
                    }
                    I += ((opcode & 0x0F00) >> 8) +1;
                    pc+=2;
                    break;
                case 0x0065:
                    for(int x = 0; x <= ((opcode & 0x0F00) >> 8);x++){
                        V[x] = memory[I + x];
                    }
                    I += ((opcode & 0x0F00) >> 8) +1;
                    pc+=2;
                    break;
            }
            break;

        default:
            std::cout << "Unknown opcode";
    }

    //Update timers
    if (delay_timer > 0) --delay_timer;
    if (sound_timer > 0) {
        if(sound_timer == 1){
            std::cout << "BEEP!";
        } //TODO add real sound
        --sound_timer;
    }
}

void Base8Chip::loadGame(string name) {
    ifstream file(name, ios::binary | ios::ate);
    streamsize size = file.tellg();

    assert(size <= (4096-512));
    file.seekg(0, ios::beg);

    vector<char> buffer(size);
    if(file.read(buffer.data(), size))
    {
        for(int x = 0;x < size;x++){
            memory[x+512] = buffer[x];
        }
    }
    file.close();
}

//Set state of a key
void Base8Chip::setKey(const unsigned char addr, const char state) {
    key[addr] = state;
}

