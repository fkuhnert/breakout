/* Trabalho por:
            Leonardo Santos: DRE:116036858
            Julio Milhm: DRE:116016387
            Davson Santos DRE:116016670

    Modificado por:
    *       Adriano Cruz (2017-1)


*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Globais.h"
#include "funcoes.h"
#include "Defs.h"
#include "NPC.h"

int main(int argc, char* args[])
{
    SDL_Rect srcRect, dstRect, srcBarsRect, dstBarsRect, srcPlayerRect, dstPlayerRect;
    int quit, curH, curW, curScreen, state, quantBroke;

    /*Start up SDL and create window*/
    if(!init()) printf("Failed to initialize!\n");
    else
    {
        /*Load media*/
        if(!loadMedia()) printf("Failed to load media!\n");
        else
        {
            /*Create NPC*/
            ball = createNPC(rand() % (SCREEN_WIDTH - IMAGE_WIDTH),
                             rand() % (SCREEN_HEIGHT - IMAGE_HEIGHT),
                             rand() % 2 ? -1: 1,
                             rand() % 2 ? -1: 1,
                             gJPGSurface);
            for(curH = 0; curH < 3; curH++)
            {
              for(curW = 0; curW < 10; curW++)
              {
                bars[curW + 10*curH] = createNPC(curW*80, curH*40, 0, 0, gBlock);
              }
            }

            player = createNPC(SCREEN_WIDTH/2 - PLAYER_WIDTH/2, SCREEN_HEIGHT - PLAYER_HEIGHT, 0, 0, gPlayer);

            /*Main loop flag*/
            quit = false;
            curScreen = SCREEN_MENU;
            Mix_PlayMusic(gMenu, -1);
            /*While application is running*/
            while(!quit)
            {
                while(curScreen == SCREEN_MENU && !quit)
                {
                  while( SDL_PollEvent( &e ) != 0 )
                  {
                    switch (e.type)
                    {
                      case SDL_QUIT:
                        quit = true;
                        break;
                      case SDL_KEYDOWN:
                        switch (e.key.keysym.sym)
                        {
                          case SDLK_ESCAPE:
                            quit = true;
                            break;
                          case SDLK_DOWN:
                            curScreen = SCREEN_GAME;
                            Mix_PlayChannel(-1, gGameBegin, 0);
                            break;
                        }
                        break;
                     }
                   }
                   SDL_FillRect(gScreenSurface, NULL,
                                SDL_MapRGB(gScreenSurface->format,
                                0x0A, 0x0A, 0x0A));

                   if(SDL_BlitSurface(ball.image, &srcRect, gScreenSurface, &dstRect) < 0)
                   {
                      printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
                      quit = true;
                   }

                   /*Update the surface*/
                   SDL_UpdateWindowSurface(gWindow);

                   /* Not so good solution, depends on your computer*/
                   SDL_Delay(5);
                }
                while(curScreen == SCREEN_GAME && !quit)
                {
                  while( SDL_PollEvent( &e ) != 0 )
                  {
                      switch (e.type)
                      {
                        case SDL_QUIT:
                          quit = true;
                          break;
                        case SDL_KEYDOWN:
                          switch (e.key.keysym.sym)
                          {
                            case SDLK_ESCAPE:
                              quit = true;
                              break;
                            case SDLK_LEFT:
                              player.stepX = -3;
                              break;
                            case SDLK_RIGHT:
                              player.stepX = 3;
                              break;
                          }
                          break;
                        case SDL_KEYUP:
                          switch (e.key.keysym.sym)
                          {
                            case SDLK_LEFT:
                              while(player.stepX < 0) player.stepX += 1;
                              break;
                            case SDLK_RIGHT:
                              while(player.stepX > 0) player.stepX -= 1;
                              break;
                          }
                          break;
                      }
                  }
                  /*Fill the surface white*/
                  SDL_FillRect(gScreenSurface, NULL,
                                SDL_MapRGB(gScreenSurface->format,
                                  0xFF, 0xFF, 0xFF ));

                  checkcollideplayer(&ball, &player);

                  moveNPC(&ball);
                  movePlayer(&player);

                  for(state = 0, curH = 0, quantBroke = 0; curH < 30; curH++)
                  {
                    if(bars[curH].draw != false) state += collisionNPC(&bars[curH], &ball);
                    else quantBroke++;
                  }
                  if(quantBroke == 30)
                  {
                    curScreen = SCREEN_MENU;
                    break;
                  }
                  if(state == 1 || state == 2) ball.stepX = -ball.stepX;
                  else if (state == 4 || state == 5 || state == 8) ball.stepY = -ball.stepY;

                  srcRect.x = 0; srcRect.y = 0;
                  srcRect.w = IMAGE_WIDTH;
                  srcRect.h = IMAGE_HEIGHT;

                  srcBarsRect.x = 0; srcBarsRect.y = 0;
                  srcBarsRect.w = 80;
                  srcBarsRect.h = 40;

                  srcPlayerRect.x = 0; srcPlayerRect.y = 0;
                  srcPlayerRect.w = PLAYER_WIDTH;
                  srcPlayerRect.h = PLAYER_HEIGHT;

                  dstRect.x = ball.posX;
                  dstRect.y = ball.posY;

                  dstPlayerRect.x = player.posX;
                  dstPlayerRect.y = player.posY;

                  if( SDL_BlitSurface(ball.image, &srcRect, gScreenSurface, &dstRect) < 0)
                  {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                  }
                  if( SDL_BlitSurface( player.image, &srcPlayerRect, gScreenSurface, &dstPlayerRect) < 0 )
                  {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                  }

                  for (curW = 0; curW < 30; curW++)
                  {
                    dstBarsRect.x = bars[curW].posX;
                    dstBarsRect.y = bars[curW].posY;
                    if(SDL_BlitSurface(bars[curW].image, &srcBarsRect, gScreenSurface, &dstBarsRect) < 0)
                    {
                      printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
                      quit = true;
                      break;
                    }
                  }
                  /*Update the surface*/
                  SDL_UpdateWindowSurface(gWindow);

                  /* Not so good solution, depends on your computer*/
                  SDL_Delay(5);
            }
         }
      }
    }
    /*Free resources and closing SDL*/
    closing();
    return 0;
}
