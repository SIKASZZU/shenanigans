#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "abilities.h"
#include "player.h"
#include "textures.h"
#include "render.h"

int main(int argc, char* argv[]) {
    SDL_SetMainReady();  // compiler ja windows bitching. Yritab muidu SDL maini kasutada
    Offset offset = {0, 0};
    
    generate_map();
    // print_map(map);

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("SDL cpp",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
    load_textures(renderer);
    
    SDL_GetWindowSize(window, &win_width, &win_height);

    player.x = (win_width / 2) - (player.size / 2);
    player.y = (win_height / 2) - (player.size / 2);
    SDL_Rect rect{player.x, player.y, player.size, player.size};
    
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    /* game loop */
    while (isRunning) {
        frame_start = SDL_GetTicks();  // framerate
        SDL_GetMouseState(&mouse_x, &mouse_y);  // a 32-bit button bitmask of the current button state. Mis tra asi see on?

        Uint32 current_tick = SDL_GetTicks(); // tickrate
        Uint32 elapsed_ticks = current_tick - previous_tick;
        previous_tick = current_tick;
        tick_lag += elapsed_ticks;
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false; 
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key_pressed = event.key.keysym.sym;
                call_set_functionality(key_pressed);
            }
        }
        
        frame_count++;
        
        while (tick_lag > tick_delay) {
            /* ticki sees peavad olema k6ik, mis fpsiga muutuks kiiremaks ja annaks eelise vs teise m2ngija suhtes. */
            tick_count++;

            tick_lag -= tick_delay;  // update tickrate
            
            update_player(offset, state);
            update_offset(offset, win_width, win_height);
            update_player_direction(player, window, offset, mouse_x, mouse_y);
            
            /* Render begin*/
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Clear with blue!
            SDL_RenderClear(renderer);  // enne uut framei, t6mba plats puhtaks
            
            load_render(renderer, tile_size, offset);
            
            draw_player_direction(renderer, player, offset, mouse_x, mouse_y);
            update_abilities(renderer, offset);
            
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // enne millegi renderimist, peab lisama rendererile colori.        
            SDL_RenderFillRect(renderer, &rect);  // player w ^^ red!!
            SDL_RenderPresent(renderer);  // dispay new frame

            /* Framerate and tickrate updating */
            if (SDL_GetTicks() - fps_timer >= 1000) {
                fps = frame_count * 1000.0f / (frame_start - fps_timer);
                ticks_per_second = tick_count * 1000.0f / (frame_start - tick_timer);

                std::cout << fps << " fps, " << ticks_per_second << " tps" << std::endl;

                fps_timer = SDL_GetTicks();
                tick_timer = SDL_GetTicks();
                frame_count = 0;
                tick_count = 0;
            }
        }

        /* Framerate cap */
        // frame_time = SDL_GetTicks() - frame_start;
        // if (frame_time < 8) { SDL_Delay(8 - frame_time); }
    }

    // Cleanup if isRunning == false
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    destroy_all_textures();
    SDL_Quit();
    IMG_Quit();
    return 0;
}
