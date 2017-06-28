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
    int curH, curW, curScreen, curSong, state, quantBroke, score, time2, avgtime,
        frametime, curOptionMenu, curOptionGame, playing;
    int hpMax = 1;
    int hpPlayer = 3;
    int time1 = 0;
    int delay = 5;
    int loopNum = 0;
    bool quit, returnGame;
    score = 0;
    if(!init()) printf("Falha em iniciar.\n");
    else
    {
        if(!loadMedia()) printf("Falha em carregar midia.\n");
        else
        {
            ball = createNPC(INIT_WIDTH, INIT_HEIGHT, 0, 1, gBall,
                             IMAGE_WIDTH, IMAGE_HEIGHT, hpPlayer);

            for(curH = 0; curH < 3; curH++)
            {
              for(curW = 0; curW < 10; curW++)
              {
                bars[curW + 10*curH] = createNPC(curW*80, curH*40, 0, 0, gBlock1,
                                                 80, 40, 0);
              }
            }
            player = createNPC(SCREEN_WIDTH/2 - PLAYER_WIDTH/2, SCREEN_HEIGHT -
                               PLAYER_HEIGHT - 2, 0, 0, gPlayer, PLAYER_WIDTH,
                               PLAYER_HEIGHT, 0);

            curSong = 10;
            quit = false;

            curScreen = SCREEN_MENU;
            while(!quit)
            {
              curOptionMenu = 0;
              while(curScreen == SCREEN_MENU && !quit)
              {
                if (curSong != 0){
                  Mix_FadeOutMusic(100);
                  Mix_PlayMusic(gMenu, -1);
                  curSong = 0;
                  playing = 0;
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
                        case SDLK_UP:
                          curOptionMenu = (curOptionMenu - 1) % 2;
                          break;
                        case SDLK_DOWN:
                          curOptionMenu = (curOptionMenu + 1) % 2;
                          break;
                        case SDLK_RETURN:
                        case SDLK_RETURN2:
                        case SDLK_SPACE:
                          if(curOptionMenu == 0)
                          {
                            curScreen = SCREEN_GAME;
                            newlevel(bars, &ball, &player, hpMax);
                            ball.hp = 3;
                            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                          }
                          else quit = true;
                          break;
                        }
                        break;
                     }
                   }
                   SDL_RenderClear(gRenderer);
                   writeTextToScreen(gTextStart, 60, 200 - textStart.imgH, textStart.imgW,
                                     textStart.imgH, &quit);
                   writeTextToScreen(gTextExit, 60, 400 - textExit.imgH, textExit.imgW,
                                     textExit.imgH, &quit);
                   if(curOptionMenu == 0)
                   {
                     writeTextToScreen(gTextArrow, 60 + textStart.imgW, 200 - textStart.imgH,
                                       textArrow.imgW, textArrow.imgH, &quit);
                   }
                   else if(curOptionMenu == 1)
                   {
                     writeTextToScreen(gTextArrow, 60 + textExit.imgW, 400 - textExit.imgH,
                                       textArrow.imgW, textArrow.imgH, &quit);
                   }
                   writeTextToScreen(gTextGrupo, 0, 600 - textGrupo.imgH, textGrupo.imgW,
                                     textGrupo.imgH, &quit);
                   SDL_RenderPresent(gRenderer);
                   SDL_Delay(5);
                }
                while(curScreen == SCREEN_GAME && !quit)
                {

                  if (time1 == 0){
                    time1 = SDL_GetTicks();
                  }
                  else{
                    time2 = SDL_GetTicks();
                    frametime += (time2 - time1);
                    time1 = 0;
                    loopNum++;
                  }
                  if (loopNum > 20){
                    avgtime = frametime/10;
                    loopNum = 0;
                    frametime = 0;
                  }
                  if (curSong == 0){
                    curSong = 1;
                  }
                  if (curSong == 1 && playing != 1){
                    Mix_FadeOutMusic(100);
                    Mix_FadeInMusic(gFase1, 10, 100);
                    playing = 1;
                  }
                  if (curSong == 2 && playing != 2){
                    Mix_FadeOutMusic(100);
                    Mix_FadeInMusic(gFase2, 10, 100);
                    playing = 2;
                  }
                  if (curSong == 3){
                    Mix_FadeOutMusic(100);
                    Mix_FadeInMusic(gFase3, 10, 100);
                    playing = 3;
                  }
                  while(SDL_PollEvent(&e) != 0)
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
                              returnGame = false;
                              curOptionGame = 0;
                              while(returnGame == false)
                              {
                                while(SDL_PollEvent(&e) != 0)
                                {
                                    switch (e.type)
                                    {
                                      case SDL_QUIT:
                                        returnGame = true;
                                        quit = true;
                                        break;
                                      case SDL_KEYDOWN:
                                        switch (e.key.keysym.sym)
                                        {
                                          case SDLK_UP:
                                            curOptionGame = (curOptionGame - 1) % 2;
                                            break;
                                          case SDLK_DOWN:
                                            curOptionGame = (curOptionGame + 1) % 2;
                                            break;
                                          case SDLK_RETURN:
                                          case SDLK_RETURN2:
                                          case SDLK_SPACE:
                                            if(curOptionGame == 0){
                                              curScreen = SCREEN_MENU;
                                              score = 0;
                                              hpPlayer = 3;
                                            }
                                          case SDLK_ESCAPE:
                                            returnGame = true;
                                            break;
                                        }
                                    }
                                }
                                SDL_RenderClear(gRenderer);
                                writeTextToScreen(gTextMenu, 400 - textMenu.imgW/2,
                                                  200 - textMenu.imgH, textMenu.imgW,
                                                  textMenu.imgH, &quit);
                                writeTextToScreen(gTextResume, 400 - textResume.imgW/2,
                                                  400 - textResume.imgH, textResume.imgW,
                                                  textResume.imgH, &quit);
                                if(curOptionGame == 0)
                                {
                                  writeTextToScreen(gTextArrow, 400 + textMenu.imgW/2,
                                                    200 - textMenu.imgH, textArrow.imgW,
                                                    textArrow.imgH, &quit);
                                }
                                else if(curOptionGame == 1)
                                {
                                  writeTextToScreen(gTextArrow, 400 + textResume.imgW/2,
                                                    400 - textResume.imgH, textArrow.imgW,
                                                    textArrow.imgH, &quit);
                                }
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(5);
                              }
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
                  if(ball.hp == 0)
                  {
                    score = 0;
                    hpMax = 1;
                    ball.hp = hpPlayer;
                    curScreen = SCREEN_MENU;
                    break;
                  }
                  checkcollideplayer(&ball, &player);

                  moveNPC(&ball);
                  movePlayer(&player);

                  for(state = 0, curH = 0, quantBroke = 0; curH < 30; curH++)
                  {
                    if(bars[curH].hp > 0) state += collisionNPC(&bars[curH], &ball, &score);
                    else quantBroke++;
                  }
                  if(quantBroke == 30)
                  {
                    score += 1000;
                    hpMax++;
                    if (hpMax > 3) hpMax = 3;
                    newlevel(bars, &ball, &player, hpMax);
                    if (curSong < 4){
                      curSong++;
                    }
                    continue;
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
                    printf("SDL nao conseguiu renderizar. Erro: %s\n", SDL_GetError());
                    quit = true;
                  }
                  if( SDL_RenderCopy(gRenderer, player.image, NULL, &dstPlayerRect) < 0 )
                  {
                    printf("SDL nao conseguiu renderizar. Erro: %s\n", SDL_GetError());
                    quit = true;
                  }
                  for (state = 0; state < 30; state++)
                  {
                    if(bars[state].hp > 0)
                    {
                      dstBarsRect.x = bars[state].posX; dstBarsRect.y = bars[state].posY;
                      dstBarsRect.w = bars[state].imgW; dstBarsRect.h = bars[state].imgH;
                      if(SDL_RenderCopy(gRenderer, bars[state].image, NULL, &dstBarsRect) < 0)
                      {
                        printf("SDL nao conseguiu renderizar. Erro: %s\n", SDL_GetError());
                        quit = true;
                        break;
                      }
                    }
                  }
                  writeTextToScreen(gTextScore, 0, 580 - textScore.imgH, textScore.imgW,
                                    textScore.imgH, &quit);
                  writeTextToScreen(gTextLives, 0, 600 - textLives.imgH, textLives.imgW,
                                    textLives.imgH, &quit);
                  showCurNumber(score, &textCurScore, gTextCurScore, textScore.imgW + 10,
                                580 - textScore.imgH, &quit);
                  showCurNumber(ball.hp, &textCurLives, gTextCurLives, textLives.imgW + 10,
                                600 - textLives.imgH, &quit);
                  SDL_RenderPresent(gRenderer);
                  if (avgtime > 14 && delay > 0){
                    delay--;
                  }
                  if (avgtime < 10 && delay < 3){
                    delay++;
                  }
                  SDL_Delay(delay);
                }
              }
            }
          }
  closing();
  return 0;
}
