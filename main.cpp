#include <iostream>
#include "Base8Chip.h"
#include <SDL2/SDL.h>
#include <assert.h>
#include <time.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 320
#define AUGMENTOR 10

SDL_Window* window = NULL;
SDL_Event e;
SDL_Renderer* render = NULL;

void setupGraphics() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) cout << "Error initializing SDL";
    window = SDL_CreateWindow("Chip8 - Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    assert(window != NULL);

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    assert(render != NULL);

}
void drawGraphics(unsigned char vec[]){

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);


    SDL_SetRenderDrawColor(render, 255,255,255,255);

    for(int w = 0;w < SCREEN_HEIGHT;w++){
        for(int h = 0;h < SCREEN_WIDTH;h++){
            if(vec[ ((w/AUGMENTOR) * 64)+ h/AUGMENTOR] == 1) {
                assert(SDL_RenderDrawPoint(render, h, w) == 0);
            }
        }
    }
    
    SDL_RenderPresent(render);

}
void close_SDL(){
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int main(int argc, char* argv[]) {

    if(argc < 2){
        cout << "Specify the ROM as argument";
        return 1;
    }
    bool quit = false;
    setupGraphics();
    Base8Chip chip;

    // Initialize the Chip8 system and load the game into the memory
    chip.init();
    chip.loadGame(argv[1]);

    // Emulation loop
    while(!quit)
    {
        if(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT) {
                break;
            }
            else if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym)
                {
                    case SDLK_1: chip.setKey(0x1, 1);break;
                    case SDLK_2: chip.setKey(0x2, 1);break;
                    case SDLK_3: chip.setKey(0x3, 1);break;
                    case SDLK_4: chip.setKey(0xC, 1);break;

                    case SDLK_q: chip.setKey(0x4, 1);break;
                    case SDLK_w: chip.setKey(0x5, 1);break;
                    case SDLK_e: chip.setKey(0x6, 1);break;
                    case SDLK_r: chip.setKey(0xD, 1);break;

                    case SDLK_a: chip.setKey(0x7, 1);break;
                    case SDLK_s: chip.setKey(0x8, 1);break;
                    case SDLK_d: chip.setKey(0x9, 1);break;
                    case SDLK_f: chip.setKey(0xE, 1);break;

                    case SDLK_z: chip.setKey(0xA, 1);break;
                    case SDLK_x: chip.setKey(0x0, 1);break;
                    case SDLK_c: chip.setKey(0xB, 1);break;
                    case SDLK_v: chip.setKey(0xF, 1);break;

                    case SDLK_ESCAPE: quit=true;break;
                }
            }
            else if(e.type == SDL_KEYUP){
                switch(e.key.keysym.sym)
                {
                    case SDLK_1: chip.setKey(0x1, 0);break;
                    case SDLK_2: chip.setKey(0x2, 0);break;
                    case SDLK_3: chip.setKey(0x3, 0);break;
                    case SDLK_4: chip.setKey(0xC, 0);break;

                    case SDLK_q: chip.setKey(0x4, 0);break;
                    case SDLK_w: chip.setKey(0x5, 0);break;
                    case SDLK_e: chip.setKey(0x6, 0);break;
                    case SDLK_r: chip.setKey(0xD, 0);break;

                    case SDLK_a: chip.setKey(0x7, 0);break;
                    case SDLK_s: chip.setKey(0x8, 0);break;
                    case SDLK_d: chip.setKey(0x9, 0);break;
                    case SDLK_f: chip.setKey(0xE, 0);break;

                    case SDLK_z: chip.setKey(0xA, 0);break;
                    case SDLK_x: chip.setKey(0x0, 0);break;
                    case SDLK_c: chip.setKey(0xB, 0);break;
                    case SDLK_v: chip.setKey(0xF, 0);break;
                }
            }
        }

        //clock_t t = clock();
        chip.emulateCycle();
        //clock_t end = clock();
        //cout << "Exec time: " << (double(end - t) / CLOCKS_PER_SEC);

        // If the draw flag is set, update the screen
        if(chip.drawFlag) {
            drawGraphics(chip.gfx);
            chip.drawFlag = false;
        }
    }
    close_SDL();

    return 0;
}


