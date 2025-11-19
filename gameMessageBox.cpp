//
// Created by Chris Foster on 1/6/18.
//

#include "gameMessageBox.h"
#include <algorithm>

gameMessageBox::gameMessageBox() {
    font = TTF_OpenFont("fonts/Vera.ttf", 24);
    if (font == nullptr) {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
    }
}

gameMessageBox::~gameMessageBox() {
    // Clean up
    TTF_CloseFont(font);
}


/****************************************************************************
 * Show message in a filled rectangle.
 * @param renderer
 * @param msg Array of message lines. The last line should be null.
 * @param x Box location
 * @param y
 * @param w Box size
 * @param h
 ***************************************************************************/
void gameMessageBox::ShowMessageBox(SDL_Renderer *renderer, const std::string& text, int x, int y, int w, int h) const {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = h;
    rect.w = w;


    SDL_Color textColor = { backColor.r, backColor.g, backColor.b, backColor.a };
    // Create a surface from the text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface == nullptr) {
        fprintf(stderr, "Error creating text surface: %s\n", TTF_GetError());
        return;
    }

    // Create a texture from the surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (textTexture == nullptr) {
        fprintf(stderr, "Error creating text texture: %s\n", SDL_GetError());
        return;
    }

    // Get the dimensions of the text
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Calculate the position to center the text within the rectangle
    int textX = x + (w - textWidth) / 2;
    int textY = y + (h - textHeight) / 2;

    // Render the text to the screen
    SDL_Rect destRect = { textX, textY, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    // Clean up
    SDL_DestroyTexture(textTexture);
}

/**************************************************************************************************************
 *
 */
void gameMessageBox::displayMultilineMessage(SDL_Renderer* renderer, const std::string& message, int x, int y, int width, int height,
                             SDL_Color textColor, SDL_Color backgroundColor) {
    // 1. Break the message into lines
    std::vector<std::string> lines;
    std::string currentLine;
    for (char c : message) {
        if (c == '\n') {
            lines.push_back(currentLine);
            currentLine.clear();
        } else {
            currentLine += c;
        }
    }
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    // 2. Calculate line height and text position
    int lineHeight = 24; // Adjust this for your font size
    int lineSpacing = 5; // Adjust for spacing between lines

    // 3. Draw the filled rectangle
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_Rect rect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &rect);

    // 4. Draw each line of text
    SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
    SDL_Texture* textTexture;
    SDL_Rect textRect;
    SDL_Surface* textSurface;
    for (size_t i = 0; i < lines.size(); ++i) {
        textSurface = TTF_RenderText_Solid(font, lines[i].c_str(), textColor); // Assuming you have TTF_Font loaded
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        textRect.x = x + (width - textRect.w) / 2; // Center text horizontally
        textRect.y = y + i * (lineHeight + lineSpacing); // Position vertically
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }
}