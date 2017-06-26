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

/*Create NPC*/
NPC createNPC(int, int, int, int, SDL_Texture *, int, int, int);

/*Move NPC*/
void moveNPC(NPC *);

void movePlayer(NPC *);

void checkcollideplayer(NPC *, NPC *);

int collisionNPC(NPC *, NPC *, int *);

void checkspeed(NPC *);

#endif

void newlevel(NPC *bars, NPC *circle, NPC *p, int hpMax);
