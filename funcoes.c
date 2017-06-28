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
        Mix_PlayChannel(-1, gHpMinus, 0);
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

/*Cria NPC*/
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
    bool success = true;
    srand(time(NULL));

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL nao foi inicializado. Erro: %s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("Breakout!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL) {
            printf("Janela nao foi criada. Erro: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL)
            {
                printf("SDL_Renderer nao foi inicializado. Erro: %s\n", SDL_GetError());
                success = false;
            }
            else SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

            if( TTF_Init() == -1 )
            {
               printf("SDL_ttf nao foi inicializado. Erro: %s\n", TTF_GetError());
               success = false;
            }
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;

            if(!(IMG_Init(imgFlags) & imgFlags))
            {
                printf("SDL_image nao foi inicializado. Erro: %s\n", IMG_GetError());
                success = false;
            }
            if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                printf("SDL_mixer nao foi inicializado. Erro: %s\n", Mix_GetError());
                success = false;
            }
        }
    }
    return success;
}

bool loadMedia() {
    bool success = true;

    gFontMenu = TTF_OpenFont("./assets/fonts/COMIC.TTF", 26);
    gFontGrupo = TTF_OpenFont("./assets/fonts/COMIC.TTF", 14);
    gFontJogo = TTF_OpenFont("./assets/fonts/COMIC.TTF", 18);
    if (gFontMenu == NULL && gFontGrupo == NULL && gFontJogo == NULL)
    {
      printf("Erro carregando fonte. Erro: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      SDL_Color color = {0x0A, 0x0A, 0x0A};
      gTextStart = loadText("Jogar", color, &textStart, gFontMenu);
      gTextExit = loadText("Sair", color, &textExit, gFontMenu);
      gTextArrow = loadText(" < ", color, &textArrow, gFontMenu);
      gTextLives = loadText("Vidas ", color, &textLives, gFontJogo);
      gTextScore = loadText("Score ", color, &textScore, gFontJogo);
      gTextMenu = loadText("Voltar ao menu", color, &textMenu, gFontMenu);
      gTextResume = loadText("Resumir", color, &textResume, gFontMenu);
      gTextGrupo = loadText("Grupo: Felipe V. Kuhnert, Gabriel M. Pinheiro, Jose Lucas Filippi",
                            color, &textGrupo, gFontGrupo);
    }
    gBall = loadTexture("./assets/images/circle.png");
    gBlock1 = loadTexture("./assets/images/block1.png");
    gBlock2 = loadTexture("./assets/images/block2.png");
    gBlock3 = loadTexture("./assets/images/block3.png");
    gPlayer = loadTexture("./assets/images/player.png");

    if( gBall == NULL || gBlock1 == NULL || gBlock2 == NULL || gBlock3 == NULL || gPlayer == NULL) {
        printf("Falha ao carregar imagem. Erro: %s\n", SDL_GetError());
        success = false;
    }
    gBottom = Mix_LoadWAV("./assets/sounds/hitbottom.wav");
    if( gBottom == NULL ){
        printf("Falha ao carregar som. Erro: %s\n", Mix_GetError());
        success = false;
    }
    gWall = Mix_LoadWAV("./assets/sounds/hitwall.wav");
    if( gWall == NULL ){
        printf("Falha ao carregar som. Erro: %s\n", Mix_GetError());
        success = false;
    }
    gTop = Mix_LoadWAV("./assets/sounds/hittop.wav");
    if( gTop == NULL ){
        printf("Falha ao carregar som. Erro: %s\n", Mix_GetError());
        success = false;
    }
    gBlockHit = Mix_LoadWAV("./assets/sounds/hitblock.wav");
    if( gBlockHit == NULL ){
        printf("Falha ao carregar som. Erro: %s\n", Mix_GetError());
        success = false;
    }
    gHpMinus = Mix_LoadWAV("./assets/sounds/hpminus.wav");
    if( gHpMinus == NULL ){
        printf("Falha ao carregar som. Erro: %s\n", Mix_GetError());
        success = false;
    }
    gMenu = Mix_LoadMUS("./assets/sounds/mus_menu.wav");
    if( gMenu == NULL ){
        printf("Falha ao carregar som. Erro: %s\n", Mix_GetError());
        success = false;
    }
    gFase1 = Mix_LoadMUS("./assets/sounds/mus_fase1.wav");
    if( gFase1 == NULL ){
        printf("Falha ao carregar som. Erro: %s\n", Mix_GetError());
        success = false;
    }
    gFase2 = Mix_LoadMUS("./assets/sounds/mus_fase2.wav");
    if( gFase2 == NULL ){
        printf("Falha ao carregar som. Erro: %s\n", Mix_GetError());
        success = false;
    }
    gFase3 = Mix_LoadMUS("./assets/sounds/mus_fase3.wav");
    if( gTop == NULL ){
        printf("Falha ao carregar som. Erro: %s\n", Mix_GetError());
        success = false;
    }
    return success;
}

void closing() {
    /*Destroi textura das imagens utilizadas no jogo*/
    SDL_DestroyTexture(gBall);
    SDL_DestroyTexture(gBlock1);
    SDL_DestroyTexture(gBlock2);
    SDL_DestroyTexture(gBlock3);
    SDL_DestroyTexture(gPlayer);
    gBall = NULL;
    gPlayer = NULL;
    gBlock1 = NULL;
    gBlock2 = NULL;
    gBlock3 = NULL;

    /*Destroi janela e o renderer*/
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    gWindow = NULL;

    /*Libera memoria das variaveis de som*/
    Mix_FreeChunk(gBottom);
    Mix_FreeChunk(gTop);
    Mix_FreeChunk(gWall);
    Mix_FreeChunk(gBlockHit);
    Mix_FreeChunk(gHpMinus);
    Mix_FreeMusic(gFase3);
    Mix_FreeMusic(gFase2);
    Mix_FreeMusic(gFase1);
    Mix_FreeMusic(gMenu);
    gBottom = NULL;
    gTop = NULL;
    gWall = NULL;
    gBlockHit = NULL;
    gHpMinus = NULL;
    gMenu = NULL;
    gFase1 = NULL;
    gFase2 = NULL;
    gFase3 = NULL;

    /*Destroi texturas dos textos utilizados no jogo*/
    SDL_DestroyTexture(gTextStart);
    SDL_DestroyTexture(gTextExit);
    SDL_DestroyTexture(gTextLives);
    SDL_DestroyTexture(gTextScore);
    SDL_DestroyTexture(gTextCurScore);
    SDL_DestroyTexture(gTextCurLives);
    SDL_DestroyTexture(gTextMenu);
    SDL_DestroyTexture(gTextResume);
    SDL_DestroyTexture(gTextGrupo);
    SDL_DestroyTexture(gTextArrow);
    gTextStart = NULL;
    gTextExit = NULL;
    gTextLives = NULL;
    gTextScore = NULL;
    gTextCurScore = NULL;
    gTextCurLives = NULL;
    gTextMenu = NULL;
    gTextResume = NULL;
    gTextGrupo = NULL;
    gTextArrow = NULL;

    /*Libera a fonte utilizada*/
    TTF_CloseFont(gFontGrupo);
    TTF_CloseFont(gFontJogo);
    TTF_CloseFont(gFontMenu);
    gFontGrupo = NULL;
    gFontMenu = NULL;
    gFontJogo = NULL;

    /*Termina os subsistemas do SDL*/
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(char *path)
{
    /*Textura da imagem a ser carregada*/
    SDL_Texture* newTexture = NULL;
    /*Carrega a imagem em uma superficie*/
    SDL_Surface* loadedSurface = IMG_Load(path);
    /*Se houve falha, mostre o erro*/
    if(loadedSurface == NULL) {
        printf("Nao foi possivel carregar a imagem %s! Erro: %s\n", path, IMG_GetError());
    }
    else
    {
        /*Introduzindo o colorkey*/
        colorkey = SDL_MapRGB (loadedSurface->format, 0xFF, 0xFF, 0xFF);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, colorkey);
        /*Cria-se textura a partir da superficie da imagem*/
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        /*Se houve falha, mostre o erro*/
        if(newTexture == NULL) {
            printf("Nao foi possivel criar a textura de %s! Erro: %s\n", path, SDL_GetError());
        }
        /*Libera o espaco da superficie carregada*/
        SDL_FreeSurface(loadedSurface);
    }
    /*Retorna textura pronta*/
    return newTexture;
}

SDL_Texture* loadText(char* textureText, SDL_Color color, NPC* text, TTF_Font* font)
{
    /*Textura do texto*/
    SDL_Texture* newText;
    /*Superficie contendo o texto*/
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText, color);
    /*Se houve falha, mostre o erro*/
    if(textSurface == NULL)
    {
        printf("Unable to load font! Erro: %s\n", SDL_GetError());
    }
    else
    {
        /*Cria a textura pela superficie do texto*/
        newText = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        /*Se houve falha, mostre o erro*/
        if(newText == NULL) {
            printf( "Nao foi possivel criar a texto! Erro: %s\n", SDL_GetError() );
        }
        /*Fornece a largura e altura desejada do texto ao NPC correspodente*/
        text->imgW = textSurface->w; text->imgH = textSurface->h;
        /*Libera o espaco da superficie do texto*/
        SDL_FreeSurface(textSurface);
    }
    /*Retorna o texto pronto*/
    return newText;
}

int hitNPC(NPC *object, int op, int vel, int *score)
{
    /*Compara HP da barra, se for 1, HP = 0, caso contrario, diminua 1 deste*/
    object->hp = object->hp <= 1 ? 0 : object->hp - 1;
    switch (object->hp){
      case 0:
        /*Se HP for 0(bloco quebrado), aumente a score*/
        *score += 100;
        break;
      /*Tipo de bloco, de acordo com o HP*/
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
    /*Efeito sonoro de colisao com o bloco*/
    Mix_PlayChannel( -1, gBlockHit, 0 );
    /*Se a operacao resultar negativa, deixe-a positiva*/
    op = op < 0 ? -op : op;
    /*Se a operacao realizada for menor ou igual a velocidade horizontal da bola...*/
    if(op <= vel) return 1;
    /*Caso contrario*/
    else return 4;
}

int collisionNPC(NPC *object, NPC *circle, int *score)
{
  /*Operacao a se realizar*/
  int op;
  /*Velocidade horizontal da bola*/
  int vel = circle->stepX;
  /*Se velocidade for negativa, torne-a positiva dentro da funcao*/
  vel = vel < 0 ? -vel : vel;
  /*Se a hitbox da bola coincidir com a posicao esquerda do bloco...*/
  if(circle->posX + IMAGE_WIDTH - 5 <= object->posX + 80 && circle->posX +
    IMAGE_WIDTH - 5 >= object->posX && circle->posY + 5 <= object->posY + 40
    && circle->posY + IMAGE_HEIGHT - 5 >= object->posY)
  {
    /*Verifique se ha mais pixels horizontais dentro da bola, para determinar se colidiu do lado ou em cima/baixo*/
    op = circle->posX + IMAGE_WIDTH - 5 - object->posX;
    return hitNPC(object, op, vel, score);
  }
  /*Caso contrario, checa se a hitbox da bola coincide com a posicao direita do bloco*/
  else if(circle->posX + 5 >= object->posX && circle->posX + 5 <= object->posX + 80
    && circle->posY + 5 <= object->posY + 40
    && circle->posY + IMAGE_HEIGHT - 5 >= object->posY)
  {
    /*Verifique se ha mais pixels horizontais dentro da bola, para determinar se colidiu do lado ou em cima/baixo*/
    op = circle->posX + 5 - (object->posX + 80);
    return hitNPC(object, op, vel, score);
  }
  /*Se nao colidir...*/
  return 0;
}

void newlevel(NPC *bars, NPC *circle, NPC *p, int hpMax){
  int scan, random;
  for (scan = 0; scan < 30; scan++){
    random = rand()%100;
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

void writeTextToScreen(SDL_Texture* text, int x, int y, int w, int h, bool* quit)
{
  /*Variavel que ira conter posicao do texto e suas medidas*/
  SDL_Rect dstTextRect;
  /*Passa posicao e medidas a variavel*/
  dstTextRect.x = x; dstTextRect.y = y;
  dstTextRect.w = w; dstTextRect.h = h;
  /*Renderiza para tela o texto, se houve erro, avise e torne quit para true*/
  if(SDL_RenderCopy(gRenderer, text, NULL, &dstTextRect) < 0)
  {
    printf("SDL nao conseguiu renderizar. Erro: %s\n", SDL_GetError());
    *quit = true;
  }
}

void showCurNumber(int number, NPC* text, SDL_Texture* numberText, int x, int y, bool* quit)
{
  /*Cor do texto*/
  SDL_Color color = {0x0A, 0x0A, 0x0A};
  /*String que ira guardar texto desejado*/
  char string[10];
  /*Coloca o numero inteiro dentro da string*/
  snprintf(string, 10, "%d", number);
  /*Transforma string em uma textura*/
  numberText = loadText(string, color, text, gFontGrupo);
  /*Escreve textura para a tela*/
  writeTextToScreen(numberText, x, y, text->imgW, text->imgH, quit);
}

void writeName(SDL_Event *e, char* nome, int *i, bool* quit)
{
  while( SDL_PollEvent(e) != 0 )
  {
    switch (e->type)
    {
      case SDL_KEYDOWN:
        switch (e->key.keysym.sym)
        {
          case SDLK_0:
            nome[*i] = '0';
            *i += 1;
            break;
          case SDLK_1:
            nome[*i] = '1';
            *i += 1;
            break;
          case SDLK_2:
            nome[*i] = '2';
            *i += 1;
            break;
          case SDLK_3:
            nome[*i] = '3';
            *i += 1;
            break;
          case SDLK_4:
            nome[*i] = '4';
            *i += 1;
            break;
          case SDLK_5:
            nome[*i] = '5';
            *i += 1;
            break;
          case SDLK_6:
            nome[*i] = '6';
            *i += 1;
            break;
          case SDLK_7:
            nome[*i] = '7';
            *i += 1;
            break;
          case SDLK_8:
            nome[*i] = '8';
            *i += 1;
            break;
          case SDLK_9:
            nome[*i] = '9';
            *i += 1;
            break;
          case SDLK_a:
            nome[*i] = 'A';
            *i += 1;
            break;
          case SDLK_b:
            nome[*i] = 'B';
            *i += 1;
            break;
          case SDLK_c:
            nome[*i] = 'C';
            *i += 1;
            break;
          case SDLK_d:
            nome[*i] = 'D';
            *i += 1;
            break;
          case SDLK_e:
            nome[*i] = 'E';
            *i += 1;
            break;
          case SDLK_f:
            nome[*i] = 'F';
            *i += 1;
            break;
          case SDLK_g:
            nome[*i] = 'G';
            *i += 1;
            break;
          case SDLK_h:
            nome[*i] = 'H';
            *i += 1;
            break;
          case SDLK_i:
            nome[*i] = 'I';
            *i += 1;
            break;
          case SDLK_j:
            nome[*i] = 'J';
            *i += 1;
            break;
          case SDLK_k:
            nome[*i] = 'K';
            *i += 1;
            break;
          case SDLK_l:
            nome[*i] = 'L';
            *i += 1;
            break;
          case SDLK_m:
            nome[*i] = 'M';
            *i += 1;
            break;
          case SDLK_n:
            nome[*i] = 'N';
            *i += 1;
            break;
          case SDLK_o:
            nome[*i] = 'O';
            *i += 1;
            break;
          case SDLK_p:
            nome[*i] = 'P';
            *i += 1;
            break;
          case SDLK_q:
            nome[*i] = 'Q';
            *i += 1;
            break;
          case SDLK_r:
            nome[*i] = 'R';
            *i += 1;
            break;
          case SDLK_s:
            nome[*i] = 'S';
            *i += 1;
            break;
          case SDLK_t:
            nome[*i] = 'T';
            *i += 1;
            break;
          case SDLK_u:
            nome[*i] = 'U';
            *i += 1;
            break;
          case SDLK_v:
            nome[*i] = 'V';
            *i += 1;
            break;
          case SDLK_w:
            nome[*i] = 'W';
            *i += 1;
            break;
          case SDLK_x:
            nome[*i] = 'X';
            *i += 1;
            break;
          case SDLK_y:
            nome[*i] = 'Y';
            *i += 1;
            break;
          case SDLK_z:
            nome[*i] = 'Z';
            *i += 1;
            break;
        }
        break;
      case SDL_QUIT:
        *quit = true;
        break;
    }
  }
}
