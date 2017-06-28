#ifndef GLOBAIS_H
#define GLOBAIS_H

extern const int SCREEN_MENU;
extern const int SCREEN_GAME;

extern SDL_Event e;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

extern SDL_Texture* gBall;
extern SDL_Texture* gBlock1;
extern SDL_Texture* gBlock2;
extern SDL_Texture* gBlock3;
extern SDL_Texture* gPlayer;
extern SDL_Texture* gBackground;

extern SDL_Texture* gTextStart;
extern SDL_Texture* gTextExit;
extern SDL_Texture* gTextLives;
extern SDL_Texture* gTextScore;
extern SDL_Texture* gTextCurScore;
extern SDL_Texture* gTextCurLives;
extern SDL_Texture* gTextMenu;
extern SDL_Texture* gTextResume;
extern SDL_Texture* gTextGrupo;
extern SDL_Texture* gTextArrow;

extern Mix_Chunk *gBottom;
extern Mix_Chunk *gTop;
extern Mix_Chunk *gWall;
extern Mix_Chunk *gBlockHit;
extern Mix_Music *gMenu;
extern Mix_Music *gFase1;
extern Mix_Music *gFase2;
extern Mix_Music *gFase3;

extern TTF_Font *gFontJogo;
extern TTF_Font *gFontMenu;
extern TTF_Font *gFontGrupo;

extern uint32_t colorkey;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int IMAGE_WIDTH;
extern const int IMAGE_HEIGHT;
extern const int PLAYER_WIDTH;
extern const int PLAYER_HEIGHT;

extern const int INIT_WIDTH;
extern const int INIT_HEIGHT;

extern SDL_Event e;

#endif
