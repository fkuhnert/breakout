/*
 * funcoes.c
 *
 * Copyright 2016 Adriano Cruz <adriano@nce.ufrj.br>
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Defs.h"
#include "Globais.h"
#include "funcoes.h"
#include "NPC.h"

void moveNPC(NPC *p) {
    p->posX += p->stepX;
    p->posY += p->stepY;
    if ((p->posX + IMAGE_WIDTH > SCREEN_WIDTH) || (p->posX < 0))
    {
        Mix_PlayChannel( -1, gWall, 0 );
        p->stepX = -p->stepX;
        p->posX += p->stepX;
    }
    if (p->posY + IMAGE_HEIGHT > SCREEN_HEIGHT)
    {
        Mix_PlayChannel(-1, gBottom, 0);
        p->hp = p->hp - 1 < 0 ? 0 : p->hp - 1;
        p->posX = INIT_WIDTH;
        p->posY = INIT_HEIGHT;
        p->stepY = 1;
        p->stepX = 0;
    }
    else if (p->posY < 0)
    {
      Mix_PlayChannel( -1, gTop, 0 );
      p->stepY = -p->stepY;
      p->posY += p->stepY;
    }
}

void checkcollideplayer(NPC *circle, NPC *p){
  float dist, coeff;
  if (circle->posY + circle->stepY + IMAGE_HEIGHT > p->posY && ((circle->posX + IMAGE_WIDTH > p->posX && circle->posX + IMAGE_WIDTH < p->posX + PLAYER_WIDTH) ||
      (circle->posX > p->posX && circle->posX < p->posX + PLAYER_WIDTH)))
      {
        Mix_PlayChannel(-1, gBottom, 0);
        dist = (circle->posX + IMAGE_WIDTH/2) - (p->posX + PLAYER_WIDTH/2);
        coeff = dist/20;
        circle->stepX += coeff;
        if (circle->stepX > 4) circle->stepX = 4;
        else if (circle->stepX < -4) circle->stepX = -4;
        checkspeed(circle);
        circle->stepY = -circle->stepY;
        circle->posY += circle->stepY;
      }
}

void checkspeed(NPC *circle){
  circle->stepY = 3-abs(circle->stepX);
  if (circle->stepY <= 0) circle->stepY = 1;
}

void movePlayer(NPC *p){
    if ((p->posX + PLAYER_WIDTH + p->stepX <= SCREEN_WIDTH) && (p->posX + p->stepX >= 0)){
      p->posX += p->stepX;
    }
}

/*Create NPC*/
NPC createNPC(int posX, int posY, int stepX, int stepY, SDL_Texture *image,
              int imgW, int imgH, int hp)
{
    NPC p;
    p.posX = posX;
    p.posY = posY;
    p.stepX = stepX;
    p.stepY = stepY;
    p.imgW = imgW;
    p.imgH = imgH;
    p.image = image;
    p.hp = hp;
    return p;
}

bool init() {
    /*Initialization flag*/
    bool success = true;

    /*Sets the rand seed to the current time*/
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
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL)
            {
                printf("SDL_Renderer nao foi inicializado. SDL_Renderer: %s\n", SDL_GetError());
                success = false;
            }
            else SDL_SetRenderDrawColor(gRenderer, 0x0A, 0x0A, 0x0A, 0x0A);

            if( TTF_Init() == -1 )
               {
                   printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                   success = false;
               }

            /*Initialize JPG and PNG loading */
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
            {
                printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                success = false;
            }
        }
    }
    return success;
}

bool loadMedia() {
    /*Loading success flag*/
    bool success = true;

    gFont = TTF_OpenFont("./assets/fonts/COMIC.TTF", 30);
    if (gFont == NULL)
    {
      printf("Erro carregando fonte. Erro: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      SDL_Color color = {0xFF, 0xFF, 0xFF};
      gTextStart = loadText("Start", color, &textStart);
      gTextExit = loadText("Exit", color, &textExit);
      gTextHighscore = loadText("Highscore ", color, &textHighscore);
      gTextLives = loadText("Lives ", color, &textLives);
      gTextScore = loadText("Score ", color, &textScore);
      gTextMenu = loadText("Back to menu", color, &textMenu);
      gTextResume = loadText("Resume", color, &textResume);
    }
    /*Load PNG surface*/
    gBall = loadTexture("./assets/images/circle.png");
    gBlock = loadTexture("./assets/images/block.png");
    gBlock1 = loadTexture("./assets/images/block1.png");
    gBlock2 = loadTexture("./assets/images/block2.png");
    gBlock3 = loadTexture("./assets/images/block3.png");
    gPlayer = loadTexture("./assets/images/player.png");
    gBackground = loadTexture("./assets/images/background.png");

    /*Loads audio files*/
    if( gBall == NULL || gBlock == NULL || gPlayer == NULL) {
        printf( "Failed to load image! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    gBottom = Mix_LoadWAV("./assets/sounds/hitbottom.wav");
    if( gBottom == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gWall = Mix_LoadWAV("./assets/sounds/hitwall.wav");
    if( gWall == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gTop = Mix_LoadWAV("./assets/sounds/hittop.wav");
    if( gTop == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gBlockHit = Mix_LoadWAV("./assets/sounds/hitblock.wav");
    if( gBlockHit == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gGameBegin = Mix_LoadWAV("./assets/sounds/gamebegin.wav");
    if( gGameBegin == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gMenu = Mix_LoadMUS("./assets/sounds/mus_menu.wav");
    if( gMenu == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gFase1 = Mix_LoadMUS("./assets/sounds/mus_fase1.wav");
    if( gFase1 == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gFase2 = Mix_LoadMUS("./assets/sounds/mus_fase2.wav");
    if( gFase2 == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gFase3 = Mix_LoadMUS("./assets/sounds/mus_fase3.wav");
    if( gTop == NULL ){
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    return success;
}

void closing() {
    /*Free loaded image*/
    SDL_DestroyTexture( gBall );
    SDL_DestroyTexture( gBlock );
    SDL_DestroyTexture( gBlock1 );
    SDL_DestroyTexture( gBlock2 );
    SDL_DestroyTexture( gBlock3 );
    SDL_DestroyTexture( gPlayer );
    gBall = NULL;
    gPlayer = NULL;
    gBlock = NULL;
    gBlock1 = NULL;
    gBlock2 = NULL;
    gBlock3 = NULL;

    /*Destroy window*/
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    /*Destroy audio*/
    Mix_FreeChunk(gBottom);
    Mix_FreeChunk(gTop);
    Mix_FreeChunk(gWall);
    Mix_FreeChunk(gBlockHit);
    Mix_FreeChunk( gGameBegin );
    Mix_FreeMusic( gFase3 );
    Mix_FreeMusic( gFase2 );
    Mix_FreeMusic( gFase1 );
    Mix_FreeMusic( gMenu );
    gBottom = NULL;
    gTop = NULL;
    gWall = NULL;
    gBlockHit = NULL;
    gGameBegin = NULL;
    gMenu = NULL;
    gFase1 = NULL;
    gFase2 = NULL;
    gFase3 = NULL;

    /*Quit SDL subsystems*/
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture( char *path )
{
    /*The final optimized image*/
    SDL_Texture* newTexture = NULL;

    /*Load image at specified path*/
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        colorkey = SDL_MapRGB (loadedSurface->format, 0xFF, 0xFF, 0xFF);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, colorkey);
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if( newTexture == NULL ) {
            printf( "Nao foi possivel criar a textura de %s! Erro: %s\n", path, SDL_GetError() );
        }
        /*Get rid of old loaded surface*/
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

SDL_Texture* loadText(char* textureText, SDL_Color color, NPC* text)
{
    SDL_Texture* newText;
    /*Load image at specified path*/
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText, color);
    if(textSurface == NULL)
    {
        printf( "Unable to load font! Erro: %s\n", SDL_GetError() );
    }
    else
    {
        newText = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if(newText == NULL) {
            printf( "Nao foi possivel criar a texto! Erro: %s\n", SDL_GetError() );
        }
        text->imgW = textSurface->w; text->imgH = textSurface->h;
        /*Get rid of old loaded surface*/
        SDL_FreeSurface(textSurface);
    }
    return newText;
}

int hitNPC(NPC *object, int op, int vel, int *score)
{
    object->hp = object->hp <= 1 ? 0 : object->hp - 1;
    switch (object->hp){
      case 0:
        *score += 100;
        break;
      case 1:
        object->image = gBlock1;
        break;
      case 2:
        object->image = gBlock2;
        break;
      case 3:
        object->image = gBlock3;
        break;
    }
    Mix_PlayChannel( -1, gTop, 0 );
    op = op < 0 ? -op : op;
    if(op <= vel) return 1;
    else return 4;
}

int collisionNPC(NPC *object, NPC *circle, int *score)
{
  int op;
  int vel = circle->stepX;
  vel = vel < 0 ? -vel : vel;
  if(circle->posX + IMAGE_WIDTH - 5 <= object->posX + 80 && circle->posX +
    IMAGE_WIDTH - 5 >= object->posX && circle->posY + 5 <= object->posY + 40
    && circle->posY + IMAGE_HEIGHT - 5 >= object->posY)
  {
    op = circle->posX + IMAGE_WIDTH - 5 - object->posX;
    return hitNPC(object, op, vel, score);
  }
  else if(circle->posX + 5 >= object->posX && circle->posX + 5 <= object->posX + 80
    && circle->posY + 5 <= object->posY + 40
    && circle->posY + IMAGE_HEIGHT - 5 >= object->posY)
  {
    op = circle->posX + 5 - (object->posX + 80);
    return hitNPC(object, op, vel, score);
  }
  return 0;
}

void newlevel(NPC *bars, NPC *circle, NPC *p, int hpMax){
  int scan, random;
  for (scan = 0; scan < 30; scan++){
    random = rand()%100;
    printf("Random = %d\n", random);
    if (random > 20) bars[scan].hp = 1;
    if (random <= 20 && random > 7) bars[scan].hp = 2;
    if (random <= 7 && random >= 0) bars[scan].hp = 3;
    if (bars[scan].hp > hpMax) bars[scan].hp = 1;
    bars[scan].hp = (rand() % hpMax + 1);
    switch (bars[scan].hp){
      case 1:
        bars[scan].image = gBlock1;
        break;
      case 2:
        bars[scan].image = gBlock2;
        break;
      case 3:
        bars[scan].image = gBlock3;
        break;
    }
  }
  circle->posX = INIT_WIDTH; circle->posY = INIT_HEIGHT;
  circle->stepX = 0; circle->stepY = 1;

  p->posX = SCREEN_WIDTH/2 - PLAYER_WIDTH/2; p->posY = SCREEN_HEIGHT - PLAYER_HEIGHT - 2;
  p->stepX = 0;
}

void writeName(SDL_Event *e, char* nome)
{
  int i = 0;
  while (i < 3)
  {
    while( SDL_PollEvent(e) != 0 )
    {
      switch (e->type)
      {
        case SDL_KEYDOWN:
          switch (e->key.keysym.sym)
          {
            case SDLK_0:
              nome[i] = '0';
              i++;
              break;
            case SDLK_1:
              nome[i] = '1';
              i++;
              break;
            case SDLK_2:
              nome[i] = '2';
              i++;
              break;
            case SDLK_3:
              nome[i] = '3';
              i++;
              break;
            case SDLK_4:
              nome[i] = '4';
              i++;
              break;
            case SDLK_5:
              nome[i] = '5';
              i++;
              break;
            case SDLK_6:
              nome[i] = '6';
              i++;
              break;
            case SDLK_7:
              nome[i] = '7';
              i++;
              break;
            case SDLK_8:
              nome[i] = '8';
              i++;
              break;
            case SDLK_9:
              nome[i] = '9';
              i++;
              break;
            case SDLK_a:
              nome[i] = 'A';
              i++;
              break;
            case SDLK_b:
              nome[i] = 'B';
              i++;
              break;
            case SDLK_c:
              nome[i] = 'C';
              i++;
              break;
            case SDLK_d:
              nome[i] = 'D';
              i++;
              break;
            case SDLK_e:
              nome[i] = 'E';
              i++;
              break;
            case SDLK_f:
              nome[i] = 'F';
              i++;
              break;
            case SDLK_g:
              nome[i] = 'G';
              i++;
              break;
            case SDLK_h:
              nome[i] = 'H';
              i++;
              break;
            case SDLK_i:
              nome[i] = 'I';
              i++;
              break;
            case SDLK_j:
              nome[i] = 'J';
              i++;
              break;
            case SDLK_k:
              nome[i] = 'K';
              i++;
              break;
            case SDLK_l:
              nome[i] = 'L';
              i++;
              break;
            case SDLK_m:
              nome[i] = 'M';
              i++;
              break;
            case SDLK_n:
              nome[i] = 'N';
              i++;
              break;
            case SDLK_o:
              nome[i] = 'O';
              i++;
              break;
            case SDLK_p:
              nome[i] = 'P';
              i++;
              break;
            case SDLK_q:
              nome[i] = 'Q';
              i++;
              break;
            case SDLK_r:
              nome[i] = 'R';
              i++;
              break;
            case SDLK_s:
              nome[i] = 'S';
              i++;
              break;
            case SDLK_t:
              nome[i] = 'T';
              i++;
              break;
            case SDLK_u:
              nome[i] = 'U';
              i++;
              break;
            case SDLK_v:
              nome[i] = 'V';
              i++;
              break;
            case SDLK_w:
              nome[i] = 'W';
              i++;
              break;
            case SDLK_x:
              nome[i] = 'X';
              i++;
              break;
            case SDLK_y:
              nome[i] = 'Y';
              i++;
              break;
            case SDLK_z:
              nome[i] = 'Z';
              i++;
              break;
          }
       }
    }
  }
}
