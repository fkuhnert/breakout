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
extern NPC textHighscores;
extern NPC textGrupo;
extern NPC textLives;
extern NPC textCurLives;
extern NPC textScore;
extern NPC textCurScore;
extern NPC textMenu;
extern NPC textResume;
extern NPC textArrow;

#endif
