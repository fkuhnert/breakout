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

const int SCREEN_MENU = 0;
const int SCREEN_GAME = 1;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = 31;
const int IMAGE_HEIGHT = 31;
const int PLAYER_WIDTH = 160;
const int PLAYER_HEIGHT = 30;

const int INIT_WIDTH = 384;
const int INIT_HEIGHT = 284;

SDL_Event e;

SDL_Window* gWindow = NULL;

NPC ball;
NPC bars[30];
NPC player;
NPC textStart;
NPC textExit;
NPC textHighscores;
NPC textLives;
NPC textCurLives;
NPC textScore;
NPC textCurScore;
NPC textMenu;
NPC textResume;
NPC textGrupo;
NPC textArrow;

uint32_t colorkey;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* gBall = NULL;
SDL_Texture* gBlock = NULL;
SDL_Texture* gBlock1 = NULL;
SDL_Texture* gBlock2 = NULL;
SDL_Texture* gBlock3 = NULL;
SDL_Texture* gPlayer = NULL;
SDL_Texture* gBackground = NULL;

SDL_Texture* gTextStart = NULL;
SDL_Texture* gTextExit = NULL;
SDL_Texture* gTextHighscores = NULL;
SDL_Texture* gTextLives = NULL;
SDL_Texture* gTextScore = NULL;
SDL_Texture* gTextCurScore = NULL;
SDL_Texture* gTextCurLives = NULL;
SDL_Texture* gTextMenu = NULL;
SDL_Texture* gTextResume = NULL;
SDL_Texture* gTextGrupo = NULL;
SDL_Texture* gTextArrow = NULL;

Mix_Chunk *gBottom = NULL;
Mix_Chunk *gTop = NULL;
Mix_Chunk *gWall = NULL;
Mix_Chunk *gBlockHit = NULL;
Mix_Chunk *gGameBegin = NULL;
Mix_Music *gMenu = NULL;
Mix_Music *gFase1 = NULL;
Mix_Music *gFase2 = NULL;
Mix_Music *gFase3 = NULL;

TTF_Font *gFontJogo = NULL;
TTF_Font *gFontMenu = NULL;
TTF_Font *gFontGrupo = NULL;
