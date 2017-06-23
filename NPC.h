#ifndef NPC_H
#define NPC_H

typedef struct _NPC {
    int posX;
    int posY;
    int stepX;
    int stepY;
    SDL_Surface* image;
    int imgW;
    int imgH;
    bool draw;
} NPC;

extern NPC ball;
extern NPC bars[30];

#endif
