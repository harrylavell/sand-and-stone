#include <stdio.h>
#include <SDL.h>
#include "structs.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CELL_SIZE 8
#define CELL_ROWS SCREEN_WIDTH / CELL_SIZE
#define CELL_COLS SCREEN_HEIGHT / CELL_SIZE

App* app;
int grid[CELL_ROWS][CELL_COLS];
int running = 1;

void init(void);

int main()
{
    app = malloc(sizeof(app));
    init();

    while (running)
    {

    }
    
	return 0;
}

void init()
{
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialise SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app->window = SDL_CreateWindow("Sand And Stone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (!app->window)
    {
        printf("Failed  to create SDL window: %s\n", SDL_GetError);
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);

    if (!app->renderer)
    {
        printf("Failed  to create SDL renderer: %s\n", SDL_GetError);
        exit(1);
    }
}