#ifndef NPC_H
#define NPC_H

typedef struct _NPC {
    int posX;
    int posY;
    int stepX;
    int stepY;
    int imgW;
    int imgH;
    SDL_Texture* image;
    int hp;
} NPC;

extern NPC ball;
extern NPC bars[30];
extern NPC player;

#endif
