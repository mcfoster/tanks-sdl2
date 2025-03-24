//
// Created by chris on 1/1/18.
//
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef TANKS2_DRAWTEXT_H
#define TANKS2_DRAWTEXT_H


class DrawText {
// Font vars
protected:
    bool initialized = false;
    int fontSize = 22;
    SDL_Color fColor = {0x00, 0x1F, 0x00};
    char *FontFile = NULL;
    TTF_Font* font = TTF_OpenFont(FontFile, fontSize);
    void setFont(TTF_Font *font);
    void fontInit();
    SDL_Rect fontRect;

public:
    DrawText();
    virtual ~DrawText();

    void setFontSize(int fontSize);
    void setFColor(const SDL_Color &fColor);
    void setFontFile(char *FontFile);
    void printText(SDL_Renderer *renderer, char *c, int x, int y);

}; // class DrawText


#endif //TANKS2_DRAWTEXT_H
