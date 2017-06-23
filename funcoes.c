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

void collisionNPC(NPC *brick, NPC *circle)
{
  if(circle->posX + IMAGE_WIDTH <= brick->posX + 80 && circle->posX +
     IMAGE_WIDTH >= brick->posX && circle->posY <= brick->posY + 40
     && circle->posY + IMAGE_HEIGHT >= brick->posY)
  {
    brick->draw = false;
    brick->posX = SCREEN_WIDTH;
    brick->posY = SCREEN_HEIGHT;
    circle->stepX = -circle->stepX;
    circle->posX += circle->stepX;
  }
  else if(circle->posX >= brick->posX && circle->posX <= brick->posX + 80
     && circle->posY <= brick->posY + 40
     && circle->posY + IMAGE_HEIGHT >= brick->posY)
  {
    brick->draw = false;
    brick->posX = SCREEN_WIDTH;
    brick->posY = SCREEN_HEIGHT;
    circle->stepX = -circle->stepX;
    circle->posX += circle->stepX;
  }
  else if(circle->posX + IMAGE_WIDTH >= brick->posX && circle->posX <=
     brick->posX + 80 && circle->posY <= brick->posY + IMAGE_HEIGHT
     && circle->posY + 40 > brick->posY)
  {
    brick->draw = false;
    brick->posX = SCREEN_WIDTH;
    brick->posY = SCREEN_HEIGHT;
    circle->stepY = -circle->stepY;
    circle->posY += circle->stepY;
  }
  else if(circle->posX + IMAGE_WIDTH >= brick->posX && circle->posX <=
     brick->posX + 80 && circle->posY + IMAGE_HEIGHT >= brick->posY
     && circle->posY < brick->posY + 40)
  {
    brick->draw = false;
    brick->posX = SCREEN_WIDTH;
    brick->posY = SCREEN_HEIGHT;
    circle->stepY = -circle->stepY;
    circle->posY += circle->stepY;
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
        gWindow = SDL_CreateWindow( "SDL Tutorial: 06_moving_images", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
    colorkey = SDL_MapRGB (gJPGSurface -> format, 0xFF, 0xFF, 0xFF);
    SDL_SetColorKey(gJPGSurface, SDL_TRUE, colorkey);
    if( gJPGSurface == NULL || gBlock == NULL) {
        printf( "Failed to load image! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    gBottom = Mix_LoadWAV("./hitbottom.wav");
    if( gBottom == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gWall = Mix_LoadWAV("./hitwall.wav");
    if( gWall == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gTop = Mix_LoadWAV("./hittop.wav");
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
    gBottom = NULL;
    gTop = NULL;
    gWall = NULL;

    /*Quit SDL subsystems*/
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
