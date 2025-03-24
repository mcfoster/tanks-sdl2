//
// Created by chris on 1/1/18.
//

#include "DrawText.h"


DrawText::DrawText(){
    if(FontFile == NULL) {
        FontFile = strdup("fonts/LiberationSerif-Bold.ttf");
    }
    TTF_Quit();
    TTF_Init();
}

DrawText::~DrawText() {
    TTF_Quit();
}

void DrawText::setFontSize(int fontSize) {
    DrawText::fontSize = fontSize;
    initialized = false;
}

void DrawText::setFColor(const SDL_Color &fColor) {
    DrawText::fColor = fColor;
}

void DrawText::setFontFile(char *FontFile) {
    DrawText::FontFile = FontFile;
    font = TTF_OpenFont(FontFile, fontSize);
}

void DrawText::setFont(TTF_Font *font) {
    DrawText::font = font;
}

void DrawText::fontInit() {
    font = TTF_OpenFont(FontFile, fontSize);
    initialized = true;
}

/****************************************************************************
 * Print the designated string at the specified coordinates
 * Parameters:
 *   sImage - Target surface where text will be printed.
 *   szText - Text to be printed.
 *   x, y - location on image where text will be drawn
 ***************************************************************************/
void DrawText::printText(SDL_Renderer *renderer, char *szText, int x, int y){
    SDL_Surface* fontSurface;
    if(!initialized)
        fontInit();
    fontSurface = TTF_RenderText_Solid(font, szText, fColor);
    fontRect.x = x;
    fontRect.y = y;
    fontRect.h = fontSurface->h;
    fontRect.w = fontSurface->w;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, fontSurface);
    SDL_RenderCopy(renderer, texture, nullptr, &fontRect);

    // SDL_BlitSurface(fontSurface, NULL, sImage, &fontRect);
    SDL_FreeSurface(fontSurface);
}

// printText

