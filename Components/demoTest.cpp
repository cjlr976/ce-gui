#include <iostream>
#include <SDL3/SDL.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    std::cout << "SDL_Init succeeded\n";
    SDL_Quit();
    return 0;
}