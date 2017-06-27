#ifndef FUNCOES_H
#define FUNCOES_H

#include "Defs.h"
#include "Globais.h"
#include "NPC.h"

bool init();

bool loadMedia();

void closing();

SDL_Texture* loadTexture(char *);

SDL_Texture* loadText(char*, SDL_Color, NPC*, TTF_Font*);

NPC createNPC(int, int, int, int, SDL_Texture *, int, int, int);

void moveNPC(NPC *);

void movePlayer(NPC *);

void checkcollideplayer(NPC *, NPC *);

int collisionNPC(NPC *, NPC *, int *);

int hitNPC(NPC *, int, int, int *);

void checkspeed(NPC *);

void newlevel(NPC *, NPC *, NPC *, int);

void writeTextToScreen(SDL_Texture*, int, int, int, int, bool*);

void showCurScore(int score, NPC* text, SDL_Texture* numberText, int x, int y, bool* quit);

void showCurLives(int lives, NPC* text, SDL_Texture* numberText, int x, int y, bool* quit);

void writeName(SDL_Event *, char*, int*, bool*);

#endif
