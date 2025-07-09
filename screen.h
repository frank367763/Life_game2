#pragma once

#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <vector>

class G {
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Event event;
    std::vector<SDL_FPoint> points;
    std::vector<SDL_Color> colors;

public:
    G()
    {
        SDL_Init(SDL_INIT_VIDEO);

        window = SDL_CreateWindow("Game of Life",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640 * 1, 480 * 1, SDL_WINDOW_ALLOW_HIGHDPI);

        renderer = SDL_CreateRenderer(window, NULL, 0);
        SDL_RenderSetScale(renderer, 1, 1);
    }

    ~G()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void drawpixel(int x, int y, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255)
    {
        points.push_back(SDL_FPoint{ (float)x, (float)y });
        colors.push_back(SDL_Color{ r, g, b, a });
    }

    void clearpixels()
    {
        points.clear();
        colors.clear();
    }

    void update()
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);

        for (size_t i = 0; i < points.size(); ++i)
        {
            SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            SDL_RenderDrawPointF(renderer, points[i].x, points[i].y);
        }

        SDL_RenderPresent(renderer);
    }

    void input()
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                SDL_Quit();
                exit(0);
            }
        }
    }
};
