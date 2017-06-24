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
int init();

/*Loads media*/
int loadMedia();

/*Frees media and shuts down SDL*/
void closing();

/*Loads individual image*/
SDL_Surface* loadSurface( char *path );

/*Create NPC*/
NPC createNPC( int posX, int posY, int stepX, int stepY, SDL_Surface *image);

/*Move NPC*/
void moveNPC(NPC *p);

/*Check collision*/
int collisionNPC(NPC *obj1, NPC *obj2);

#endif
