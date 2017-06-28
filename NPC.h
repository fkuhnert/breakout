#ifndef NPC_H
#define NPC_H

/*Estrutura utilizada por objetos e texto*/
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

/*Objetos do jogo*/
extern NPC ball;
extern NPC bars[30];
extern NPC player;

/*Texto do jogo*/
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
