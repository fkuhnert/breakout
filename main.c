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
#include <math.h>
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
    SDL_Rect dstRect, dstBarsRect, dstPlayerRect;
    int quit, curH, curW, curScreen, curSong, state, quantBroke, hpMax;


    /*Start up SDL and create window*/
    if(!init()) printf("Failed to initialize!\n");
    else
    {
        /*Load media*/
        if(!loadMedia()) printf("Failed to load media!\n");
        else
        {
            /*Create NPC*/
            ball = createNPC(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, 1, gBall,
                             IMAGE_WIDTH, IMAGE_HEIGHT, 0);

            for(hpMax = 2, curH = 0; curH < 3; curH++)
            {
              for(curW = 0; curW < 10; curW++)
              {
                bars[curW + 10*curH] = createNPC(curW*80, curH*40, 0, 0, gBlock,
                                                 80, 40, rand() % hpMax + 1);
                printf("%d\n", bars[curW + 10*curH].hp);
              }
            }

            player = createNPC(SCREEN_WIDTH/2 - PLAYER_WIDTH/2, SCREEN_HEIGHT -
                               PLAYER_HEIGHT - 2, 0, 0, gPlayer, PLAYER_WIDTH,
                               PLAYER_HEIGHT, 0);

            /*Main loop flag*/
            quit = false;
            curScreen = SCREEN_MENU;
            /*While application is running*/
            while(!quit)
            {
              Mix_FadeOutMusic(100);
              Mix_PlayMusic(gMenu, -1);
              curSong = 0;

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
                            for(state = 0; state < 30; state++)
                            {
                              bars[state].draw = true;
                              bars[state].hp = (rand() % hpMax + 1);
                            }
                            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                            break;
                        }
                        break;
                     }
                   }
                   SDL_RenderClear(gRenderer);
                   SDL_RenderPresent(gRenderer);
                   SDL_Delay(5);
                }
                while(curScreen == SCREEN_GAME && !quit)
                {
                  if (curSong == 0){
                    Mix_FadeOutMusic(100);
                    Mix_FadeInMusic(gFase1, 1, 100);
                    curSong = 1;
                  }
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
                    hpMax++;
                    SDL_SetRenderDrawColor(gRenderer, 0x0A, 0x0A, 0x0A, 0xFF);
                    break;
                  }
                  if(state == 1 || state == 2) ball.stepX = -ball.stepX;
                  else if (state == 4 || state == 5 || state == 8) ball.stepY = -ball.stepY;

                  dstRect.x = ball.posX; dstRect.y = ball.posY;
                  dstRect.w = ball.imgW; dstRect.h = ball.imgH;

                  dstPlayerRect.x = player.posX; dstPlayerRect.y = player.posY;
                  dstPlayerRect.w = player.imgW; dstPlayerRect.h = player.imgH;

                  SDL_RenderClear(gRenderer);
                  if( SDL_RenderCopy(gRenderer, ball.image, NULL, &dstRect ) < 0 )
                  {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                  }
                  if( SDL_RenderCopy(gRenderer, player.image, NULL, &dstPlayerRect) < 0 )
                  {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                  }
                  for (state = 0; state < 30; state++)
                  {
                    if(bars[state].draw != 0)
                    {
                      dstBarsRect.x = bars[state].posX; dstBarsRect.y = bars[state].posY;
                      dstBarsRect.w = bars[state].imgW; dstBarsRect.h = bars[state].imgH;
                      if(SDL_RenderCopy(gRenderer, bars[state].image, NULL, &dstBarsRect) < 0)
                      {
                        printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
                        quit = true;
                        break;
                      }
                    }
                  }
                  /*Update the surface*/
                  SDL_RenderPresent(gRenderer);
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
