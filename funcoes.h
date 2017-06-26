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
SDL_Texture* loadTexture( char *path );

/*Create NPC*/
NPC createNPC(int posX, int posY, int stepX, int stepY, SDL_Texture *image,
              int imgW, int imgH, int hp);

/*Move NPC*/
void moveNPC(NPC *p);

void movePlayer(NPC *p);

void checkcollideplayer(NPC *circle, NPC *player);

int collisionNPC(NPC *obj1, NPC *obj2);

void checkspeed(NPC *p);

#endif
