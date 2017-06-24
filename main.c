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
#include "Globais.h"
#include "funcoes.h"
#include "Defs.h"
#include "NPC.h"

int main( int argc, char* args[] ) {
    SDL_Rect srcRect, dstRect, srcBarsRect, dstBarsRect;
    int quit, curH, curW, state;

    /*Event handler*/
    SDL_Event e;

    /*Start up SDL and create window*/
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    }
    else {
        /*Load media*/
        if( !loadMedia() ) {
            printf( "Failed to load media!\n" );
        }
        else {
            /*Create NPC*/
            Begin:
            ball = createNPC(rand() % (SCREEN_WIDTH - IMAGE_WIDTH),
                             300 + rand() % (SCREEN_HEIGHT - 300) - IMAGE_HEIGHT,
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

            /*Main loop flag*/
            quit = false;

            /*While application is running*/
            while( !quit ) {
                while( SDL_PollEvent( &e ) != 0 )
                {
                    switch (e.type)
                    {
                        case SDL_QUIT:
                            quit = true;
                            break;
                        case SDL_KEYDOWN:
                            if (e.key.keysym.sym == SDLK_ESCAPE) quit = true;
                            if (e.key.keysym.sym == SDLK_UP)
                            {
                              ball.stepX += 1;
                              ball.stepY += 1;
                            }
                            if (e.key.keysym.sym == SDLK_DOWN)
                            {
                              ball.stepX -= 1;
                              ball.stepY -= 1;
                            }
                            if (e.key.keysym.sym == SDLK_r) goto Begin;
                            break;
                    }
                }

                /*Fill the surface white*/
                SDL_FillRect( gScreenSurface, NULL,
                              SDL_MapRGB( gScreenSurface->format,
                              0xFF, 0xFF, 0xFF ) );

                moveNPC(&ball);
                for(state = 0, curH = 0; curH < 30; curH++)
                {
                  if(bars[curH].draw != false) state += collisionNPC(&bars[curH], &ball);
                }

                if(state == 1 || state == 2) ball.stepX = -ball.stepX;
                else if (state == 4 || state == 5 || state == 8) ball.stepY = -ball.stepY;

                srcRect.x = 0; srcRect.y = 0;
                srcRect.w = IMAGE_WIDTH;
                srcRect.h = IMAGE_HEIGHT;
                srcBarsRect.x = 0; srcBarsRect.y = 0;
                srcBarsRect.w = 80;
                srcBarsRect.h = 40;
                dstRect.x = ball.posX;
                dstRect.y = ball.posY;
                if( SDL_BlitSurface( ball.image, &srcRect, gScreenSurface, &dstRect ) < 0 )
                {
                  printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                  quit = true;
                }
                else
                {
                  for (curW = 0; curW < 30; curW++)
                  {
                    if(bars[curW].draw != 0)
                    {
                      dstBarsRect.x = bars[curW].posX;
                      dstBarsRect.y = bars[curW].posY;
                      if(SDL_BlitSurface(bars[curW].image, &srcBarsRect, gScreenSurface,
                                        &dstBarsRect) < 0)
                      {
                        printf("SDL could not blit! SDL Error: %s\n", SDL_GetError());
                        quit = true;
                        break;
                      }
                    }
                  }

                /*Update the surface*/
                SDL_UpdateWindowSurface( gWindow );

                /* Not so good solution, depends on your computer*/
                SDL_Delay(2);
                }
            }
        }
    }

    /*Free resources and closing SDL*/
    closing();

    return 0;
}
