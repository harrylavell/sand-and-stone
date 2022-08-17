#include <stdio.h>
#include <SDL.h>
#include "structs.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CELL_SIZE 4
#define CELL_ROWS SCREEN_WIDTH / CELL_SIZE
#define CELL_COLS SCREEN_HEIGHT / CELL_SIZE
#define FPS 1000

enum BlockType {
    None,
    Sand,
    Stone
};

App* app;
short grid[CELL_ROWS][CELL_COLS];
short gridNextState[CELL_ROWS][CELL_COLS];
int running = 1;
int holdingMouseButton = 0;
enum BlockType activeBlockType;

int x, y;

void init(void);
void handleInput(void);
void update(void);
void drawGrid(void);
void drawBlock(int, SDL_Rect);
void triggerBlockChange(int, int);
int coordsWithinBounds(int, int);

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

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            holdingMouseButton = 1;

            if (SDL_GetMouseState(&x, &y) & SDL_BUTTON_LMASK)
                activeBlockType = Sand;

            if (SDL_GetMouseState(&x, &y) & SDL_BUTTON_RMASK)
                activeBlockType = Stone;
            break;

        case SDL_MOUSEMOTION:
            x = event.motion.x;
            y = event.motion.y;
            break;

        case SDL_MOUSEBUTTONUP:
            holdingMouseButton = 0;
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
    // generate blocks
    if (holdingMouseButton && coordsWithinBounds(x, y)) {
        triggerBlockChange(x, y);
        triggerBlockChange(x + CELL_SIZE, y);
        triggerBlockChange(x, y + CELL_SIZE);
        triggerBlockChange(x - CELL_SIZE, y);
        triggerBlockChange(x, y - CELL_SIZE);
    }

    // perform logic
    for (int row = 0; row < CELL_ROWS; row++) {
        for (int col = 0; col < CELL_COLS; col++) {
            int blockType = grid[row][col];

            // free falling
            if (blockType == Sand && col < CELL_COLS - 1) {
                gridNextState[row][col] = None;
                gridNextState[row][col + 1] = Sand;
            }

            // landed on a sand block
            if (blockType == Sand && grid[row][col + 1] == Sand) {

                // check left
                if (grid[row - 1][col + 1] == None) {
                    gridNextState[row - 1][col + 1] = Sand;
                }

                // check right
                else if (grid[row + 1][col + 1] == None) {
                    gridNextState[row + 1][col + 1] = Sand;
                }

                // stack on top
                else {
                    gridNextState[row][col] = Sand;
                }
            }

            // landed on a stone block
            if (blockType == Sand && (grid[row][col + 1] == Stone)) {
                gridNextState[row][col] = Sand;
            }

            if (blockType == Stone) {
                gridNextState[row][col] = Stone;
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
        SDL_SetRenderDrawColor(app->renderer, 194, 178, 128, 255);
        SDL_RenderFillRect(app->renderer, &block);
    }
    else if (blockType == Stone) {
        SDL_SetRenderDrawColor(app->renderer, 126, 150, 159, 255);
        SDL_RenderFillRect(app->renderer, &block);
    }
}

void triggerBlockChange(int x, int y)
{
    int row = floor(x / CELL_SIZE);
    int col = floor(y / CELL_SIZE);
    grid[row][col] = activeBlockType;
}

int coordsWithinBounds(int x, int y)
{
    if (x >= 0 && x <= SCREEN_WIDTH)
        if (y >= 0 && y <= SCREEN_HEIGHT)
            return 1;
    return 0;
}