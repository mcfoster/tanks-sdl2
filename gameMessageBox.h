//
// Created by chris on 1/6/18.
//
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "DrawText.h"

#ifndef TANKS2_GAMEMESSAGEBOX_H
#define TANKS2_GAMEMESSAGEBOX_H

const SDL_Color backColor = {0x7F, 0x7F, 0x7F, 0xFF};
const SDL_Color forColor = {0x00, 0x00, 0x1F, 0xFF};

class gameMessageBox : DrawText {
private:
   TTF_Font* font;
public:

    gameMessageBox();
    virtual ~gameMessageBox();

    void ShowMessageBox(SDL_Renderer *renderer, const std::string& text, int x, int y, int w, int h) const;

    void displayMultilineMessage(SDL_Renderer* renderer, const std::string& message,
        int x, int y, int width, int height, SDL_Color textColor=forColor, SDL_Color backgroundColor=backColor);
};


#endif //TANKS2_GAMEMESSAGEBOX_H
