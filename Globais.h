/*
 * Globais.h
 *
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 *
 */

#ifndef GLOBAIS_H
#define GLOBAIS_H

extern const int SCREEN_MENU;
extern const int SCREEN_GAME;

extern SDL_Event e;

extern SDL_Window* gWindow;

extern SDL_Surface* gScreenSurface;

extern SDL_Surface* gJPGSurface;
extern SDL_Surface* gBlock;
extern SDL_Surface* gPlayer;

extern Mix_Chunk *gBottom;
extern Mix_Chunk *gTop;
extern Mix_Chunk *gWall;
extern Mix_Chunk *gBlockHit;
extern Mix_Chunk *gGameBegin;
extern Mix_Music *gMenu;
extern Mix_Music *gFase1;
extern Mix_Music *gFase2;
extern Mix_Music *gFase3;

extern TTF_Font *gFont;

extern uint32_t colorkey;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int IMAGE_WIDTH;
extern const int IMAGE_HEIGHT;
extern const int PLAYER_WIDTH;
extern const int PLAYER_HEIGHT;

extern SDL_Event e;

#endif
