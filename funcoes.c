/*
 * funcoes.c
 *
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "Defs.h"
#include "Globais.h"
#include "funcoes.h"
#include "NPC.h"

void moveNPC(NPC *p) {
    p->posX += p->stepX;
    p->posY += p->stepY;

    if ( (p->posX + IMAGE_WIDTH > SCREEN_WIDTH) || (p->posX < 0) ) {
        Mix_PlayChannel( -1, gWall, 0 );
        p->stepX = -p->stepX;
        p->posX += p->stepX;
    }
    if ( (p->posY + IMAGE_HEIGHT > SCREEN_HEIGHT) || (p->posY < 0) ) {
        if (p->posY + IMAGE_HEIGHT > SCREEN_HEIGHT) Mix_PlayChannel(-1, gBottom, 0);
        else Mix_PlayChannel( -1, gTop, 0 );
        p->stepY = -p->stepY;
        p->posY += p->stepY;
    }
}

void checkcollideplayer(NPC *circle, NPC *p){
  float dist;
  if (circle->posY + IMAGE_HEIGHT == p->posY && ((circle->posX + IMAGE_WIDTH > p->posX && circle->posX + IMAGE_WIDTH < p->posX + PLAYER_WIDTH) ||
      (circle->posX > p->posX && circle->posX < p->posX + PLAYER_WIDTH))){
        circle->stepY = -circle->stepY;
        Mix_PlayChannel(-1, gBottom, 0);
        dist = (circle->posX + IMAGE_WIDTH/2) - (p->posX + PLAYER_WIDTH/2);
        circle->stepX += dist/15;
        circle->stepX += p->stepX/2;
        if (circle->stepX > 4){
          circle->stepX = 4;
        }
        if (circle->stepX < -4){
          circle ->stepX = -4;
        }
      }
}

void movePlayer(NPC *p){
    if ((p->posX + PLAYER_WIDTH + p->stepX <= SCREEN_WIDTH) && (p->posX + p->stepX >= 0)){
      p->posX += p->stepX;
    }
}

/*Create NPC*/
NPC createNPC( int posX, int posY, int stepX, int stepY,
               SDL_Surface *image) {
    NPC p;

    p.posX = posX;
    p.posY = posY;
    p.stepX = stepX;
    p.stepY = stepY;
    p.image = image;
    return p;
}

int init() {
    /*Initialization flag*/
    int success = true;

    srand(time(NULL));

    /*Initialize SDL*/
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {
        /*Create window*/
        gWindow = SDL_CreateWindow( "Breakout!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            /*Initialize JPG and PNG loading */
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else {
                /*Get window surface*/
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
        }
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
    }



    return success;
}

int loadMedia() {
    /*Loading success flag*/
    int success = true;
    /*uint32_t colorKey;*/

    /*Load PNG surface*/
    gJPGSurface = loadSurface( "./circle.png" );
    gBlock = loadSurface("./block.png");
    gPlayer = loadSurface("./block.png");
    colorkey = SDL_MapRGB (gJPGSurface -> format, 0xFF, 0xFF, 0xFF);
    SDL_SetColorKey(gJPGSurface, SDL_TRUE, colorkey);
    if( gJPGSurface == NULL || gBlock == NULL || gPlayer == NULL) {
        printf( "Failed to load image! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    gBottom = Mix_LoadWAV("./Sounds/hitbottom.wav");
    if( gBottom == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gWall = Mix_LoadWAV("./Sounds/hitwall.wav");
    if( gWall == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gTop = Mix_LoadWAV("./Sounds/hittop.wav");
    if( gTop == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gMenu = Mix_LoadMUS("./Sounds/mus_menu.wav");
    if( gMenu == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gFase1 = Mix_LoadMUS("./Sounds/mus_fase1.wav");
    if( gFase1 == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gFase2 = Mix_LoadMUS("./Sounds/mus_fase2.wav");
    if( gFase2 == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gFase3 = Mix_LoadMUS("./Sounds/mus_fase3.wav");
    if( gTop == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}

void closing() {
    /*Free loaded image*/
    SDL_FreeSurface( gJPGSurface );
    gJPGSurface = NULL;
    SDL_FreeSurface( gBlock );
    gBlock = NULL;

    /*Destroy window*/
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    /*Destroy audio*/
    Mix_FreeChunk(gBottom);
    Mix_FreeChunk(gTop);
    Mix_FreeChunk(gWall);
    Mix_FreeMusic( gFase3 );
    Mix_FreeMusic( gFase2 );
    Mix_FreeMusic( gFase1 );
    Mix_FreeMusic( gMenu );
    gBottom = NULL;
    gTop = NULL;
    gWall = NULL;
    gMenu = NULL;
    gFase1 = NULL;
    gFase2 = NULL;
    gFase3 = NULL;

    /*Quit SDL subsystems*/
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface( char *path ) {
    /*The final optimized image*/
    SDL_Surface* optimizedSurface = NULL;

    /*Load image at specified path*/
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else {
        /*Convert surface to screen format*/
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL ) {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        /*Get rid of old loaded surface*/
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

int collisionNPC(NPC *object, NPC *circle)
{
  if(circle->posX + IMAGE_WIDTH - 5 <= object->posX + 80 && circle->posX +
    IMAGE_WIDTH - 5 >= object->posX && circle->posY + 5 <= object->posY + 40
    && circle->posY + IMAGE_HEIGHT - 5 >= object->posY)
  {
    printf("Conta: %d\nPosicao bola: %d,%d\nPosicao plat: %d,%d\n", circle->posX + IMAGE_WIDTH - 5 - circle->posX, circle->posY, object->posX, object->posY);
    if(circle->posX + IMAGE_WIDTH - 5 - object->posX <= 1 && circle->posX +
       IMAGE_WIDTH - 5 - object->posX >= 0)
    {
      object->draw = false;
      object->posX = SCREEN_WIDTH;
      object->posY = SCREEN_HEIGHT;
      printf("Colidiu no lado esquerdo!\n");
      return 1;
    }
    else
    {
      object->draw = false;
      object->posX = SCREEN_WIDTH;
      object->posY = SCREEN_HEIGHT;
      printf("Colidiu em baixo ou cima e a esqueda!\n");
      return 4;
    }
  }
  else if(circle->posX + 5 >= object->posX && circle->posX + 5 <= object->posX + 80
    && circle->posY + 5 <= object->posY + 40
    && circle->posY + IMAGE_HEIGHT - 5 >= object->posY)
  {
    printf("Conta: %d\nPosicao bola: %d,%d\nPosicao plat: %d,%d\n", circle->posX + 5 - (object->posX + 80), circle->posX, circle->posY, object->posX, object->posY);
    if(circle->posX + 5 - (object->posX + 80) <= 1 && circle->posX + 5 -
       (object->posX + 80) >= 0)
    {
      object->draw = false;
      object->posX = SCREEN_WIDTH;
      object->posY = SCREEN_HEIGHT;
      printf("Colidiu no lado direito!\n");
      return 1;
    }
    else
    {
      object->draw = false;
      object->posX = SCREEN_WIDTH;
      object->posY = SCREEN_HEIGHT;
      printf("Colidiu em baixo ou cima e a direita!\n");
      return 4;
    }
  }
  return 0;
}
