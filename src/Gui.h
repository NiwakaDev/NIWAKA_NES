#pragma once
#include "common.h"
#include <SDL2/SDL.h>
#include <OpenGL/OpenGL.h>
#include <SDL2/SDL_opengl.h>
using namespace std;

class JoyPad;

class Gui:public Object{
    private:
        bool quit;
        JoyPad* joy_pad;
        int SCREEN_WIDTH = WIDTH;
        int SCREEN_HEIGHT = HEIGHT;
        SDL_Window* window = NULL;
        SDL_Renderer *renderer = NULL;
        SDL_Texture *texture   = NULL;
        void HandleKeyUp(SDL_Event *e);
        void HandleKeyDown(SDL_Event *e);
        BUTTON_KIND SdlScancode2KeyCode(SDL_Event *e);
        bool button_state[BUTTON_KIND_CNT];
        Pixel* image;
    public:
        Gui(JoyPad* joy_pad);
        void Update();
        void PollEvents();
        bool IsQuit();
        void SetPixel(int x, int y, Pixel pixel);
};