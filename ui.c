
#define SDL_MAIN_USE_CALLBACKS 1  
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#include "splaytree.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string.h>

#define height 1080
#define width 1920


static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static TTF_Font *font = NULL;
static splaytree *tree;

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

void DrawThickLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, float thickness) {
    if (thickness <= 0.0f) {
        return; 
    }

    if (thickness == 1.0f) {
        SDL_RenderLine(renderer, x1, y1, x2, y2);
        return;
    }

    float dx = x2 - x1;
    float dy = y2 - y1;

    float length = sqrtf(dx * dx + dy * dy);

    if (length == 0.0f) {
        SDL_FRect rect = { x1 - thickness / 2.0f, y1 - thickness / 2.0f, thickness, thickness };
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    float nx = -dy / length;
    float ny = dx / length;

    float halfThickness = thickness / 2.0f;

    SDL_FPoint p1_offset = {x1 + nx * halfThickness, y1 + ny * halfThickness};
    SDL_FPoint p2_offset = {x1 - nx * halfThickness, y1 - ny * halfThickness};
    SDL_FPoint p3_offset = {x2 - nx * halfThickness, y2 - ny * halfThickness};
    SDL_FPoint p4_offset = {x2 + nx * halfThickness, y2 + ny * halfThickness};

    SDL_Vertex vertices[4];
    SDL_FColor color; 

    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    color.r = (float)r / 255.0f;
    color.g = (float)g / 255.0f;
    color.b = (float)b / 255.0f;
    color.a = (float)a / 255.0f;


    vertices[0].position = p1_offset;
    vertices[0].color = color;
    vertices[0].tex_coord = (SDL_FPoint){0.0f, 0.0f};

    vertices[1].position = p2_offset;
    vertices[1].color = color;
    vertices[1].tex_coord = (SDL_FPoint){0.0f, 1.0f};

    vertices[2].position = p3_offset;
    vertices[2].color = color;
    vertices[2].tex_coord = (SDL_FPoint){1.0f, 1.0f};
    
    vertices[3].position = p4_offset;
    vertices[3].color = color;
    vertices[3].tex_coord = (SDL_FPoint){1.0f, 0.0f};

    int indices[] = {0, 1, 2, 0, 2, 3};

    SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);
}

void DrawTree(SDL_Renderer* renderer, splaytree *tree, TTF_Font* font, SDL_Color textColor, float x, float y, float hgap, float parent_x, float parent_y){
    if (tree == NULL) return;
    DrawCircle(renderer, x,y,30,10,font,tree->data, textColor);
    if (!(parent_x == -1.0f && parent_y == -1.0f)) {
        float angle = atan2f(y - parent_y, x - parent_x);

        float line_start_x = parent_x + 30 * cosf(angle);
        float line_start_y = parent_y + 30 * sinf(angle);

        float line_end_x = x - 30 * cosf(angle); 
        float line_end_y = y - 30 * sinf(angle);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 
        DrawThickLine(renderer, line_start_x, line_start_y, line_end_x, line_end_y, 3.0f); 
}
    DrawTree(renderer, tree->left, font, textColor, x - hgap, y+100, hgap*0.5f, x, y);
    DrawTree(renderer, tree->right, font, textColor, x+ hgap, y+100,  hgap * 0.5f, x, y);
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
    
    tree = create_node(10);
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
    char input[100];
    char command[10];
    int number;
    printf("Enter command (e.g., Add 5 or Delete 3 or Rand 50): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (sscanf(input, "%s %d", command, &number) == 2) {
            if (strcmp(command, "Add") == 0) {
                tree = insert_node(tree, number);
            } else if (strcmp(command, "Del") == 0) {
                printf("lmao");
            } else {
                printf("Unknown command: %s\n", command);
            }
        } else {
            printf("Invalid input format. Please enter a command followed by a number.\n");
        }
    }
    DrawTree(renderer, tree, font, textColor, 960, 80, 480, -1.0f, -1.0f);
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;  
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
