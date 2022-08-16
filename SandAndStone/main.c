#include <stdio.h>
#include <SDL.h>
#include "structs.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CELL_SIZE 8
#define CELL_ROWS SCREEN_WIDTH / CELL_SIZE
#define CELL_COLS SCREEN_HEIGHT / CELL_SIZE
#define FPS 30

enum BlockType {
    None,
    Sand,
    Stone
};

App* app;
int grid[CELL_ROWS][CELL_COLS];
int gridNextState[CELL_ROWS][CELL_COLS];
int running = 1;
enum BlockType activeBlockType;

void init(void);
void handleInput(void);
void update(void);
void drawGrid(void);
void drawBlock(int, SDL_Rect);
void triggerBlockChange(int, int);

int main()
{
    app = malloc(sizeof(app));
    init();

    while (running)
    {
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
        
        handleInput();

        update();

        SDL_RenderClear(app->renderer);
        drawGrid();
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
                activeBlockType = Sand;

            if (SDL_GetMouseState(&x, &y) & SDL_BUTTON_RMASK)
                activeBlockType = Stone;

            triggerBlockChange(x, y);
            break;

        case SDL_QUIT:
            running = 0;
            break;

        default:
            break;
        }
    }
}

void update(void)
{
    // perform logic
    for (int row = 0; row < CELL_ROWS; row++) {
        for (int col = 0; col < CELL_COLS; col++) {
            int blockType = grid[row][col];

            if (blockType == Sand) {
                gridNextState[row][col] = None;
                gridNextState[row][col + 1] = Sand;
            }

        }
    }

    // copy grid next state to the original
    for (int row = 0; row < CELL_ROWS; row++) {
        for (int col = 0; col < CELL_COLS; col++) {
            grid[row][col] = gridNextState[row][col];
        }
    }
}

void drawGrid()
{
    for (int row = 0; row < CELL_ROWS; row++) {
        for (int col = 0; col < CELL_COLS; col++) {
            SDL_Rect block;
            block.x = row * CELL_SIZE;
            block.y = col * CELL_SIZE;
            block.w = block.h = CELL_SIZE;

            drawBlock(grid[row][col], block);
        }
    }
}

void drawBlock(int blockType, SDL_Rect block)
{
    if (blockType == None) {
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 0);
        SDL_RenderFillRect(app->renderer, &block);
    }
    else if (blockType == Sand) {
        SDL_SetRenderDrawColor(app->renderer, 213, 123, 244, 255);
        SDL_RenderDrawRect(app->renderer, &block);
        SDL_RenderFillRect(app->renderer, &block);
    }
    else if (blockType == Stone) {
        SDL_SetRenderDrawColor(app->renderer, 32, 87, 98, 255);
        SDL_RenderDrawRect(app->renderer, &block);
        SDL_RenderFillRect(app->renderer, &block);
    }
}

void triggerBlockChange(int x, int y)
{
    int row = floor(x / CELL_SIZE);
    int col = floor(y / CELL_SIZE);
    grid[row][col] = activeBlockType;
}