#include "game.h"
#include <iostream>
#include "textures.h"


void render_map(SDL_Renderer* renderer, const int tile_size, struct Offset& offset,
                SDL_Texture* ground_tex, SDL_Texture* tree_tex) {
    int player_tile_y = player.y / tile_size;
    int bottom = player_tile_y + render_radius;
    int top    = player_tile_y - render_radius;

    int player_tile_x = player.x / tile_size;
    int left   = player_tile_x - render_radius;
    int right  = player_tile_x + render_radius;

    // Pass 1: Render all ground tiles
    for (int row = 0; row < map_size; row++) {
        if (row < top || row > bottom) continue;
        
        for (int column = 0; column < map_size; column++) {
            if (column < left || column > right) continue;
            
            int row_coord = row * tile_size - offset.y;
            int col_coord = column * tile_size - offset.x;
            SDL_Rect ground_tile = {col_coord, row_coord, tile_size, tile_size};

            if (map[row][column] != 0) {
                SDL_RenderCopy(renderer, ground_tex, nullptr, &ground_tile);
            }
        }
    }

    // Pass 2: Render all tree tiles w ground underneith
    for (int y = 0; y < map_size; y++) {
        if (y < top || y > bottom) continue;

        for (int x = 0; x < map_size; x++) {
            if (x < left || x > right) continue;

            if (map[y][x] == 2) {
                int row_coord = y * tile_size - offset.y;
                int col_coord = x * tile_size - offset.x;

                SDL_Rect ground_tile = {col_coord, row_coord, tile_size, tile_size};
                SDL_RenderCopy(renderer, ground_tex, nullptr, &ground_tile);
                SDL_Rect tree_tile = {
                    col_coord - (tile_size / 2),
                    row_coord - (tile_size * 2),
                    tile_size * 2,
                    tile_size * 3
                };
                SDL_RenderCopy(renderer, tree_tex, nullptr, &tree_tile);
            }
        }
    }
}


void render_map_numbers(SDL_Renderer* renderer, const int tile_size, struct Offset& offset) {
    int player_tile_y = player.y / tile_size;
    int bottom = player_tile_y + render_radius;
    int top    = player_tile_y - render_radius;

    int player_tile_x = player.x / tile_size;
    int left   = player_tile_x - render_radius;
    int right  = player_tile_x + render_radius;

    for (int y = 0; y < map_size; y++) {
        if (y < top || y > bottom) continue;

        for (int x = 0; x < map_size; x++) {
            if (x < left || x > right) continue;
            
            int row_coord = y * tile_size - offset.y;
            int col_coord = x * tile_size - offset.x;
            SDL_Rect at_tile = {col_coord, row_coord, tile_size, tile_size};
            load_specific_number(renderer, map[y][x], at_tile);
        }
    }
}


void load_render(SDL_Renderer* renderer, const int tile_size, struct Offset& offset) {
    /* Vali ise, mis mappi tahad geneda. */
    render_map(renderer, tile_size, offset, ground_tex, tree_tex);
    render_map_numbers(renderer, tile_size, offset);
}