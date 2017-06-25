/*
 * Globais.h
 *
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 *
 */

#ifndef GLOBAIS_H
#define GLOBAIS_H

/*The window we'll be rendering to*/
extern SDL_Window* gWindow;

/*The surface contained by the window*/
extern SDL_Surface* gScreenSurface;

/*Current displayed PNG image*/
extern SDL_Surface* gJPGSurface;
extern SDL_Surface* gBlock;
extern SDL_Surface* gPlayer;

/* Audio for game */
extern Mix_Chunk *gBottom;
extern Mix_Chunk *gTop;
extern Mix_Chunk *gWall;
extern Mix_Music *gMenu;
extern Mix_Music *gFase1;
extern Mix_Music *gFase2;
extern Mix_Music *gFase3;

extern uint32_t colorkey;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int IMAGE_WIDTH;
extern const int IMAGE_HEIGHT;
extern const int PLAYER_WIDTH;
extern const int PLAYER_HEIGHT;

extern SDL_Event e;

#endif
