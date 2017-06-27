/*
 * funcoes.h
 *
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 *
 */

#ifndef FUNCOES_H
#define FUNCOES_H

#include "Defs.h"
#include "Globais.h"
#include "NPC.h"

/*Starts up SDL and creates window*/
bool init();

/*Loads media*/
bool loadMedia();

/*Frees media and shuts down SDL*/
void closing();

/*Loads individual image*/
SDL_Texture* loadTexture(char *);

SDL_Texture* loadText(char* textureText, SDL_Color color, NPC* text);

/*Create NPC*/
NPC createNPC(int, int, int, int, SDL_Texture *, int, int, int);

/*Move NPC*/
void moveNPC(NPC *);

void movePlayer(NPC *);

void checkcollideplayer(NPC *, NPC *);

int collisionNPC(NPC *, NPC *, int *);

int hitNPC(NPC *object, int op, int vel);

void checkspeed(NPC *);

void newlevel(NPC *bars, NPC *circle, NPC *p, int hpMax);

void writeName(SDL_Event *e, char* nome);

#endif
