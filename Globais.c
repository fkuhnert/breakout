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
#include <SDL2/SDL_ttf.h>
#include "Globais.h"
#include "Defs.h"
#include "NPC.h"

/*Screen state*/
const int SCREEN_MENU = 0;
const int SCREEN_GAME = 1;

/*Measurements*/
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = 31;
const int IMAGE_HEIGHT = 31;
const int PLAYER_WIDTH = 160;
const int PLAYER_HEIGHT = 30;

/*Posicao inicial da bola*/
const int INIT_WIDTH = 384;
const int INIT_HEIGHT = 284;

/*Event handler*/
SDL_Event e;

/*The window we'll be rendering to*/
SDL_Window* gWindow = NULL;

/*Game objects*/
NPC ball;
NPC bars[30];
NPC player;
NPC textStart;
NPC textExit;
NPC textHighscore;
NPC textHighscores[10];
NPC textLives;
NPC textScore;
NPC textMenu;
NPC textResume;

/*Colokey for the objetcs*/
uint32_t colorkey;

/*Window renderer*/
SDL_Renderer* gRenderer = NULL;

SDL_Texture* gBall = NULL;
SDL_Texture* gBlock = NULL;
SDL_Texture* gPlayer = NULL;

SDL_Texture* gTextStart = NULL;
SDL_Texture* gTextExit = NULL;
SDL_Texture* gTextHighscore = NULL;
SDL_Texture* gTextHighscores = NULL;
SDL_Texture* gTextLives = NULL;
SDL_Texture* gTextScore = NULL;
SDL_Texture* gTextMenu = NULL;
SDL_Texture* gTextResume = NULL;

/* Audio for game */
Mix_Chunk *gBottom = NULL;
Mix_Chunk *gTop = NULL;
Mix_Chunk *gWall = NULL;
Mix_Chunk *gBlockHit = NULL;
Mix_Chunk *gGameBegin = NULL;
Mix_Music *gMenu = NULL;
Mix_Music *gFase1 = NULL;
Mix_Music *gFase2 = NULL;
Mix_Music *gFase3 = NULL;

/*Font for the in-game texts*/
TTF_Font *gFont = NULL;
