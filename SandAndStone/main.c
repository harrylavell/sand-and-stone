#include <stdio.h>
#include <SDL.h>
#include "structs.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CELL_SIZE 8
#define CELL_ROWS SCREEN_WIDTH / CELL_SIZE
#define CELL_COLS SCREEN_HEIGHT / CELL_SIZE
#define FPS 30

App* app;
int grid[CELL_ROWS][CELL_COLS];
int running = 1;

void init(void);
void handleInput(void);

int main()
{
    app = malloc(sizeof(app));
    init();

    while (running)
    {
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
        
        handleInput();

        SDL_RenderClear(app->renderer);
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / FPS);
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

void handleInput()
{
    SDL_Event event;
    int x, y;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            x = event.motion.x;
            y = event.motion.y;

            if (SDL_GetMouseState(&x, &y) & SDL_BUTTON_LMASK)
                printf("Left pressed\n");

            if (SDL_GetMouseState(&x, &y) & SDL_BUTTON_RMASK)
                printf("Right pressed\n");
            break;

        case SDL_QUIT:
            running = 0;
            break;

        default:
            break;
        }
    }
}