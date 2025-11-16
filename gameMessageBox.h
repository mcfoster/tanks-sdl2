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

    /**
     * inputText
     * -------------------------------------------------------------------------
     * Blockingly prompts the user for a line of text using SDL2 text input.
     * A small modal dialog is rendered on the provided renderer with a prompt,
     * a single-line input field, and an "OK" button. The user can type until
     * they press Enter or click the OK button. Backspace deletes characters.
     * The returned value is the entered string, or "[none]" if the user
     * confirms with an empty input.
     *
     * Requirements:
     * - Uses SDL2 text input events (SDL_StartTextInput / SDL_TEXTINPUT)
     * - Limits input to the provided maximum length
     * - Renders prompt, current input, and an OK button
     *
     * @param renderer  Valid SDL_Renderer to draw into (not null)
     * @param prompt    Prompt message displayed above the input field
     * @param length    Maximum number of characters allowed (<= 0 treated as 0)
     * @return          Entered text, or "[none]" when nothing was entered
     */
    std::string inputText(SDL_Renderer* renderer, const std::string& prompt, int length);

};


#endif //TANKS2_GAMEMESSAGEBOX_H
