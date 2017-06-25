/*
 * Globais.c
 *
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "Globais.h"
#include "Defs.h"
#include "NPC.h"

SDL_Event e;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = 31;
const int IMAGE_HEIGHT = 31;
const int PLAYER_WIDTH = 80;
const int PLAYER_HEIGHT = 40;

/*The window we'll be rendering to*/
SDL_Window* gWindow = NULL;

NPC ball;
NPC bars[30];
NPC player;
uint32_t colorkey;

/*The surface contained by the window*/
SDL_Surface* gScreenSurface = NULL;

/*Current displayed PNG image*/
SDL_Surface* gJPGSurface = NULL;
SDL_Surface* gBlock = NULL;
SDL_Surface* gPlayer = NULL;

/* Audio for game */
Mix_Chunk *gBottom = NULL;
Mix_Chunk *gTop = NULL;
Mix_Chunk *gWall = NULL;
Mix_Music *gMenu = NULL;
Mix_Music *gFase1 = NULL;
Mix_Music *gFase2 = NULL;
Mix_Music *gFase3 = NULL;
