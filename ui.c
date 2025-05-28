
#define SDL_MAIN_USE_CALLBACKS 1  
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#include "splaytree.h"
#include <SDL3_ttf/SDL_ttf.h>

#define height 1080
#define width 1920


static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static TTF_Font *font = NULL;

void DrawCircle(SDL_Renderer* renderer, float centerX, float centerY, float radius, int thickness, TTF_Font* font, int number, SDL_Color textColor)
{
    const int segments = 360;
    for (int t = 0; t < thickness; ++t)
    {
        float currentRadius = radius - (thickness / 2.0f) + t;
        for (int i = 0; i < segments; ++i)
        {
            float theta = (float)i * (2.0f * M_PI / segments);
            float x = centerX + currentRadius * cosf(theta);
            float y = centerY + currentRadius * sinf(theta);
            SDL_RenderPoint(renderer, x, y);
        }
    }
    if (font != NULL && number >= 0 && number <= 99)
    {
        char numStr[3]; 
        snprintf(numStr, sizeof(numStr), "%d", number);
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, numStr, 2,textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_DestroySurface(textSurface);

            if (textTexture) {
                float texWidth, texHeight;
                SDL_GetTextureSize(textTexture, &texWidth, &texHeight);
                SDL_FRect dstRect = {
                    .x = centerX - (texWidth / 2.0f),
                    .y = centerY - (texHeight / 2.0f),
                    .w = (float)texWidth,
                    .h = (float)texHeight
                };
                SDL_RenderTexture(renderer, textTexture, NULL, &dstRect);
                SDL_DestroyTexture(textTexture);
            }
        }
    }
    
}



void DrawTree(SDL_Renderer* renderer, splaytree *tree, TTF_Font* font, SDL_Color textColor, float x, float y, float hgap){
    if (tree == NULL) return;
    DrawCircle(renderer, x,y,30,10,font,tree->data, textColor);
    DrawTree(renderer, tree->left, font, textColor, x - hgap, y+100, hgap*0.5f);
    DrawTree(renderer, tree->right, font, textColor, x+ hgap, y+100,  hgap * 0.5f);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Splaytree-SDL3", "1.0", "Splaytree-SDL3");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Splaytree-SDL3", width, height, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    font = TTF_OpenFont("KosugiMaru-Regular.ttf", 24);
    if (!font)
    {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    
    return SDL_APP_CONTINUE; 
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  
    }
    return SDL_APP_CONTINUE;  
}


SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 50, 50,  50,   255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 250, 250,  250,   255);
    SDL_Color textColor = {255, 255, 255, 255};
    splaytree *tree = create_node(10);
    insert_node(tree, 8);
    insert_node(tree, 9);
    insert_node(tree, 20);
    insert_node(tree, 17);
    insert_node(tree, 22);
    insert_node(tree, 33);
    insert_node(tree, 45);
    insert_node(tree, 15);
    insert_node(tree, 14);
    insert_node(tree, 13);
    insert_node(tree, 19);
    DrawTree(renderer, tree, font, textColor, 960, 80, 480);
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;  
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
