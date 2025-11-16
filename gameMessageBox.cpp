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

/**************************************************************************************************************
 * inputText
 * -------------------------------------------------------------------------
 * Prompts the user to enter a single line of text using SDL2 text-input.
 * This function opens a simple modal dialog drawn into the given renderer,
 * showing a prompt, an editable single-line field, and an "OK" button.
 *
 * Interaction:
 * - Typing produces characters via SDL_TEXTINPUT events.
 * - Backspace deletes the last character.
 * - Enter/Return or clicking the OK button confirms the input.
 * - The input length is limited to the provided "length" parameter.
 *
 * Return value:
 * - The entered string if at least one character was typed.
 * - "[none]" if the user confirmed with an empty input.
 *
 * Notes:
 * - This is a blocking loop that processes SDL events until confirmation.
 * - The method assumes SDL_ttf is already initialized and a valid font is loaded.
 **************************************************************************************************************/
std::string gameMessageBox::inputText(SDL_Renderer* renderer, const std::string& prompt, int length) {
    // Defensive checks and normalization
    if (!renderer) return std::string("[none]");
    if (length < 0) length = 0;

    // Fetch renderer output size to center the dialog
    int rw = 0, rh = 0;
    SDL_GetRendererOutputSize(renderer, &rw, &rh);
    if (rw == 0 || rh == 0) { rw = 640; rh = 480; }

    // Dialog layout parameters
    const int dlgW = std::max(360, rw / 2);
    const int dlgH = 180;
    const int dlgX = (rw - dlgW) / 2;
    const int dlgY = (rh - dlgH) / 3;

    const int padding = 14;
    const int lineSpacing = 6;

    // Input field rectangle (single-line entry)
    const int fieldHeight = 36;
    const SDL_Rect inputRect{dlgX + padding, dlgY + padding + 28, dlgW - padding * 2, fieldHeight};

    // OK button rectangle
    const int btnW = 90;
    const int btnH = 34;
    const SDL_Rect okRect{dlgX + dlgW - btnW - padding, dlgY + dlgH - btnH - padding, btnW, btnH};

    // Colors used in the dialog
    SDL_Color bg = backColor;         // dialog background
    SDL_Color fg = forColor;          // dialog border and prompt color
    SDL_Color white{255, 255, 255, 255};
    SDL_Color black{0, 0, 0, 255};
    SDL_Color gray{210, 210, 210, 255};

    std::string value;                // current input buffer

    // Enable SDL text input
    SDL_StartTextInput();

    bool done = false;
    while (!done) {
        // 1) Handle events: text input, backspace, confirm, mouse click
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                // If application is asked to quit, exit input early
                done = true;
                break;
            }
            if (e.type == SDL_TEXTINPUT) {
                // Append typed characters until the maximum length is reached
                for (const char* p = e.text.text; *p; ++p) {
                    if ((int)value.size() < length) {
                        value.push_back(*p);
                    } else {
                        break;
                    }
                }
            } else if (e.type == SDL_KEYDOWN) {
                const SDL_Keycode key = e.key.keysym.sym;
                if (key == SDLK_BACKSPACE) {
                    if (!value.empty()) value.pop_back();
                } else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
                    done = true; // confirm
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    const int mx = e.button.x;
                    const int my = e.button.y;
                    // If OK button clicked, confirm
                    if (mx >= okRect.x && mx < okRect.x + okRect.w && my >= okRect.y && my < okRect.y + okRect.h) {
                        done = true;
                    }
                }
            }
        }

        // 2) Render the dialog on top of the current frame
        // Dialog background (with slight alpha to make it stand out)
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 240);
        SDL_Rect dlgRect{dlgX, dlgY, dlgW, dlgH};
        SDL_RenderFillRect(renderer, &dlgRect);

        // Border
        SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, 255);
        SDL_RenderDrawRect(renderer, &dlgRect);

        // Prompt text (single-line)
        if (font) {
            SDL_Surface* promptSurf = TTF_RenderText_Solid(font, prompt.c_str(), fg);
            if (promptSurf) {
                SDL_Texture* promptTex = SDL_CreateTextureFromSurface(renderer, promptSurf);
                int tw = promptSurf->w;
                int th = promptSurf->h;
                SDL_FreeSurface(promptSurf);
                if (promptTex) {
                    SDL_Rect dst{dlgX + padding, dlgY + padding, std::min(tw, dlgW - padding * 2), th};
                    SDL_RenderCopy(renderer, promptTex, nullptr, &dst);
                    SDL_DestroyTexture(promptTex);
                }
            }
        }

        // Input field background and border
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
        SDL_RenderFillRect(renderer, &inputRect);
        SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
        SDL_RenderDrawRect(renderer, &inputRect);

        // Current input text and caret
        if (font) {
            SDL_Surface* textSurf = TTF_RenderText_Solid(font, value.c_str(), black);
            if (textSurf) {
                SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, textSurf);
                int tw = textSurf->w;
                int th = textSurf->h;
                SDL_FreeSurface(textSurf);
                if (textTex) {
                    SDL_Rect dst{inputRect.x + 6, inputRect.y + (inputRect.h - th) / 2, std::min(tw, inputRect.w - 12), th};
                    SDL_RenderCopy(renderer, textTex, nullptr, &dst);
                    SDL_DestroyTexture(textTex);

                    // Simple caret after the text (static, non-blinking)
                    int cx = dst.x + dst.w + 1;
                    if (cx < inputRect.x + inputRect.w - 4) {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawLine(renderer, cx, inputRect.y + 6, cx, inputRect.y + inputRect.h - 6);
                    }
                }
            }
        }

        // OK button
        SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, gray.a);
        SDL_RenderFillRect(renderer, &okRect);
        SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
        SDL_RenderDrawRect(renderer, &okRect);
        if (font) {
            SDL_Surface* okSurf = TTF_RenderText_Solid(font, "OK", black);
            if (okSurf) {
                SDL_Texture* okTex = SDL_CreateTextureFromSurface(renderer, okSurf);
                int tw = okSurf->w;
                int th = okSurf->h;
                SDL_FreeSurface(okSurf);
                if (okTex) {
                    SDL_Rect dst{okRect.x + (okRect.w - tw) / 2, okRect.y + (okRect.h - th) / 2, tw, th};
                    SDL_RenderCopy(renderer, okTex, nullptr, &dst);
                    SDL_DestroyTexture(okTex);
                }
            }
        }

        // Present the dialog frame
        SDL_RenderPresent(renderer);

        // Throttle the loop a bit to reduce CPU usage
        SDL_Delay(10);
    }

    // Stop text input before returning
    SDL_StopTextInput();

    // Return entered value or "[none]" if empty
    if (value.empty()) return std::string("[none]");
    return value;
}
