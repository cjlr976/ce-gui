#include "textbox.h"
#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

TextBox::TextBox(Renderer* renderer, TTF_Font* font)
    : rendererPtr(renderer),
      font(font),
      text(),
      cursorVisible(true),
      lastCursorToggle(SDL_GetTicks()),
      cursorBlinkInterval(500),
      textScrollOffset(0.0f)
{
}

void TextBox::setBox(int x, int y, int w, int h) {
    setPosition(x, y);
    setSize(w, h);
}

void TextBox::clear() {
    text.clear();
    textScrollOffset = 0.0f;
}

SDL_Texture* TextBox::createTextTexture(int* outW, int* outH) {
    if (!font || !rendererPtr) return nullptr;

    SDL_Renderer* native = rendererPtr->getNativeRenderer();
    if (!native) return nullptr;

    const char* toRender = text.empty() ? " " : text.c_str();
    SDL_Color white = { 255, 255, 255, 255 };

    SDL_Surface* surface = TTF_RenderText_Blended(font, toRender, SDL_strlen(toRender), white);
    if (!surface) {
        std::cerr << "TTF_RenderText_Blended failed: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(native, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surface);
        return nullptr;
    }

    if (outW) *outW = surface->w;
    if (outH) *outH = surface->h;

    SDL_DestroySurface(surface);
    return texture;
}

void TextBox::draw(Renderer& renderer) {
    SDL_Renderer* native = renderer.getNativeRenderer();
    if (!native) return;

    // Draw outline
    SDL_FRect outline = { (float)posX, (float)posY, (float)width, (float)height };
    renderer.setDrawColor(255, 255, 255, 255);
    SDL_RenderRect(native, &outline);

    if (!font) return;

    // Render text
    const char* toRender = text.empty() ? " " : text.c_str();
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, toRender, strlen(toRender), white);
    if (!surf) {
        std::cerr << "TTF_RenderText_Blended failed: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(native, surf);
    if (!tex) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surf);
        return;
    }

    // Horizontal scrolling if text wider than box
    float padding = 5.0f;
    float maxW = width - padding * 2.0f;
    if ((float)surf->w > maxW) {
        textScrollOffset = (float)surf->w - maxW;
    } else {
        textScrollOffset = 0.0f;
    }

    SDL_FRect dst = {
        posX + padding - textScrollOffset,
        posY + (height - surf->h) / 2.0f,
        (float)surf->w,
        (float)surf->h
    };

    SDL_RenderTexture(native, tex, nullptr, &dst);

    // Cursor blinking
    Uint32 now = SDL_GetTicks();
    if (now - lastCursorToggle > cursorBlinkInterval) {
        cursorVisible = !cursorVisible;
        lastCursorToggle = now;
    }

    if (cursorVisible) {
        SDL_FRect cursorRect = { dst.x + dst.w + 2.0f, dst.y, 2.0f, dst.h };
        renderer.setDrawColor(255, 255, 255, 255);
        SDL_RenderFillRect(native, &cursorRect);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroySurface(surf);
}


void TextBox::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_TEXT_INPUT) {
        if (e.text.text && e.text.text[0] != '\0') {
            text += e.text.text;
        }
    } else if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.key == SDLK_BACKSPACE && !text.empty()) {
            while (!text.empty()) {
                unsigned char c = static_cast<unsigned char>(text.back());
                text.pop_back();
                if ((c & 0x80) == 0 || (c & 0xC0) == 0xC0)
                    break;
            }
        }
    }

    Widget::handleEvent(e);
}
