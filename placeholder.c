void DrawTree(SDL_Renderer* renderer, splaytree *tree, TTF_Font* font, SDL_Color textColor, int depth, int index){
    if (tree == NULL) return;
    float x, y;
    y = 80.0f + 80.0f * depth;

    int maxNodesAtLevel = (int)pow(2, depth); 
    float spacing = width / (maxNodesAtLevel + 1); 
    x = spacing * (index + 1);

    DrawCircle(renderer, x, y, 30, 10, font, tree->data, textColor);

    DrawTree(renderer, tree->left, font, textColor, depth + 1, 2 * index);
    DrawTree(renderer, tree->right, font, textColor, depth + 1, 2 * (index + 1));
    
}