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

extern NPC textStart;
extern NPC textExit;
extern NPC textHighscore;
extern NPC textHighscores[10];
extern NPC textLives;
extern NPC textScore;
extern NPC textMenu;
extern NPC textResume;

#endif
