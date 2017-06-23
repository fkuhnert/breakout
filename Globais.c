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

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = 61;
const int IMAGE_HEIGHT = 61;

/*The window we'll be rendering to*/
SDL_Window* gWindow = NULL;

NPC ball;
NPC bars[30];
uint32_t colorkey;

/*The surface contained by the window*/
SDL_Surface* gScreenSurface = NULL;

/*Current displayed PNG image*/
SDL_Surface* gJPGSurface = NULL;
SDL_Surface* gBlock = NULL;

/* Audio for game */
Mix_Chunk *gBottom = NULL;
Mix_Chunk *gTop = NULL;
Mix_Chunk *gWall = NULL;
