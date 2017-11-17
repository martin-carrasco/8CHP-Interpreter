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
bool quit = false;

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
            if(vec[ ( ( w / AUGMENTOR) * 64) + (h / AUGMENTOR)] == 1) {
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

bool update_keys(const Uint8* state, Base8Chip &chip){
    chip.setKey(0x1, state[SDL_SCANCODE_1]);
    chip.setKey(0x2, state[SDL_SCANCODE_2]);
    chip.setKey(0x3, state[SDL_SCANCODE_3]);
    chip.setKey(0xC, state[SDL_SCANCODE_4]);

    chip.setKey(0x4, state[SDL_SCANCODE_Q]);
    chip.setKey(0x5, state[SDL_SCANCODE_W]);
    chip.setKey(0x6, state[SDL_SCANCODE_E]);
    chip.setKey(0xD, state[SDL_SCANCODE_R]);

    chip.setKey(0x7, state[SDL_SCANCODE_A]);
    chip.setKey(0x8, state[SDL_SCANCODE_S]);
    chip.setKey(0x9, state[SDL_SCANCODE_D]);
    chip.setKey(0xE, state[SDL_SCANCODE_F]);

    chip.setKey(0xA, state[SDL_SCANCODE_Z]);
    chip.setKey(0x0, state[SDL_SCANCODE_X]);
    chip.setKey(0xB, state[SDL_SCANCODE_C]);
    chip.setKey(0xF, state[SDL_SCANCODE_V]);

    if(state[SDL_SCANCODE_ESCAPE])
        return false;
    return true;
}

void get_key_down(Base8Chip &chip) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_1:
                    chip.setKey(0x1, 1);
                    break;
                case SDLK_2:
                    chip.setKey(0x2, 1);
                    break;
                case SDLK_3:
                    chip.setKey(0x3, 1);
                    break;
                case SDLK_4:
                    chip.setKey(0xC, 1);
                    break;

                case SDLK_q:
                    chip.setKey(0x4, 1);
                    break;
                case SDLK_w:
                    chip.setKey(0x5, 1);
                    break;
                case SDLK_e:
                    chip.setKey(0x6, 1);
                    break;
                case SDLK_r:
                    chip.setKey(0xD, 1);
                    break;

                case SDLK_a:
                    chip.setKey(0x7, 1);
                    break;
                case SDLK_s:
                    chip.setKey(0x8, 1);
                    break;
                case SDLK_d:
                    chip.setKey(0x9, 1);
                    break;
                case SDLK_f:
                    chip.setKey(0xE, 1);
                    break;

                case SDLK_z:
                    chip.setKey(0xA, 1);
                    break;
                case SDLK_x:
                    chip.setKey(0x0, 1);
                    break;
                case SDLK_c:
                    chip.setKey(0xB, 1);
                    break;
                case SDLK_v:
                    chip.setKey(0xF, 1);
                    break;

                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
    }
}
void get_key_up(Base8Chip &chip){
    while(SDL_PollEvent(&e)){

        if(e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case SDLK_1:
                    chip.setKey(0x1, 0);
                    break;
                case SDLK_2:
                    chip.setKey(0x2, 0);
                    break;
                case SDLK_3:
                    chip.setKey(0x3, 0);
                    break;
                case SDLK_4:
                    chip.setKey(0xC, 0);
                    break;

                case SDLK_q:
                    chip.setKey(0x4, 0);
                    break;
                case SDLK_w:
                    chip.setKey(0x5, 0);
                    break;
                case SDLK_e:
                    chip.setKey(0x6, 0);
                    break;
                case SDLK_r:
                    chip.setKey(0xD, 0);
                    break;

                case SDLK_a:
                    chip.setKey(0x7, 0);
                    break;
                case SDLK_s:
                    chip.setKey(0x8, 0);
                    break;
                case SDLK_d:
                    chip.setKey(0x9, 0);
                    break;
                case SDLK_f:
                    chip.setKey(0xE, 0);
                    break;

                case SDLK_z:
                    chip.setKey(0xA, 0);
                    break;
                case SDLK_x:
                    chip.setKey(0x0, 0);
                    break;
                case SDLK_c:
                    chip.setKey(0xB, 0);
                    break;
                case SDLK_v:
                    chip.setKey(0xF, 0);
                    break;

            }
        }
    }
}

int main(int argc, char* argv[]) {

    if(argc < 2){
        cout << "Specify the ROM as argument";
        return 1;
    }
    setupGraphics();
    Base8Chip chip;

    // Initialize the Chip8 system and load the game into the memory
    chip.init();
    chip.loadGame(argv[1]);

    // Emulation loop
    while(!quit)
    {
        SDL_PumpEvents();
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if(!update_keys(state, chip))
            break;

        chip.emulateCycle();

        // If the draw flag is set update the screen
        if(chip.drawFlag) {
            drawGraphics(chip.gfx);
            chip.drawFlag = false;
        }
    }
    //cout << "Freeing SDL memory";
    close_SDL();
    //cout << "Freed";

    return 0;
}


