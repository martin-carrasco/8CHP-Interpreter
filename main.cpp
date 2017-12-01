#include <iostream>
#include "Base8Chip.h"
#include <SDL2/SDL.h>
#include <assert.h>
#include <time.h>
#include <thread>

#define INFO_SCREEN_HEIGHT 320

#define GAME_SCREEN_WIDTH  640
#define GAME_SCREEN_HEIGHT 320


#define AUGMENTOR 10

SDL_Window* window = NULL;
SDL_Event e;
SDL_Renderer* render = NULL;
SDL_Texture* texture = NULL;
bool quit = false;

void setupGraphics() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) cout << "Error initializing SDL";
    window = SDL_CreateWindow("Chip8 - Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT/* + INFO_SCREEN_HEIGHT*/, SDL_WINDOW_SHOWN);
    assert(window != NULL);

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture( render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);

    assert(texture != NULL);
    assert(render != NULL);

}
void draw_mem(char* mem, unsigned short pc){
    //Limpia el cuadrado de informacion
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_Rect rec;
    rec.x = GAME_SCREEN_HEIGHT + 16;
    rec.y = 16;
    rec.w = GAME_SCREEN_WIDTH - 32;
    rec.h = INFO_SCREEN_HEIGHT - 32;

    SDL_RenderDrawRect(render, &rec);

    for(int x = pc-10;x < pc+11;x++){
    }


    //Imprime 10 locaciones en la memoria, la actual y diez locaciones despues
    for(int h = GAME_SCREEN_HEIGHT + 16;h < GAME_SCREEN_HEIGHT+INFO_SCREEN_HEIGHT-16;h++){
        for(int w = 16;w < GAME_SCREEN_WIDTH - 16;w++){

        }
    }

    unsigned short opcode = mem[pc] << 8 | mem[pc + 1];

}
void draw_frame(){
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 255,255,255,255);
    for(int h = GAME_SCREEN_HEIGHT; h < INFO_SCREEN_HEIGHT + GAME_SCREEN_HEIGHT;h++){
        for(int w = 0;w < GAME_SCREEN_WIDTH;w++){
            if(h < (GAME_SCREEN_HEIGHT + 15) || GAME_SCREEN_HEIGHT+INFO_SCREEN_HEIGHT-h < 15)
                SDL_RenderDrawPoint(render, w, h);
            if(w < 15 || GAME_SCREEN_WIDTH - w < 15)
                SDL_RenderDrawPoint(render, w, h);
        }
    }
    SDL_RenderPresent(render);
}
void drawGraphics(unsigned char vec[]){

    //Draws graphics on the texture, swaps the render target copies the texture to the render and presents it
    SDL_SetRenderTarget(render, texture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    for(int w = 0;w < GAME_SCREEN_HEIGHT;w++){
        for(int h = 0;h < GAME_SCREEN_WIDTH;h++){
            if(vec[ ( ( w / AUGMENTOR) * 64) + (h / AUGMENTOR)] == 1) {
                assert(SDL_RenderDrawPoint(render, h, w) == 0);
            }
        }
    }
    SDL_SetRenderTarget(render, NULL);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
}
void close_SDL(){
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool handle_input(Base8Chip& chip, SDL_Event& event){
    switch (event.type)
    {
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
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
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
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
            break;
    }
    return true;
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
    //draw_frame();
    SDL_Event event;

    // Emulation loop
    while(!quit)
    {

        chip.emulateCycle();

        //Gets memory to print out locations
        //char* mem = new char[4096];
        //chip.get_mem(mem);

        //unsigned short pc = chip.get_pc();

        //Dibuja la posicion actual de la memoria
        //draw_mem(mem, pc);


        // / If the draw flag is set update the screen
        if(chip.drawFlag) {
            drawGraphics(chip.gfx);
            chip.drawFlag = false;
        }

        SDL_PollEvent(&event);

        if(!handle_input(chip, event))
            break;

    }
    //cout << "Freeing SDL memory";
    close_SDL();
    //cout << "Freed";

    return 0;
}


