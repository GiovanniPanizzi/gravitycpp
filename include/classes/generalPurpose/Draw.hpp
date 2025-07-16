#pragma once
#include "Window.hpp"
#include "../../utilities/globals.hpp"
#include "../../objects/Galaxy.hpp"

class Draw {
    private:
    Window& window;

    public:
    Draw(Window& window);
    virtual ~Draw();
    void clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawFilledRotatedRect(int x, int y, int w, int h, double angleRad, int pivotX, int pivotY, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawFilledCircle(int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawAnnularSectionWithPalette(
    int centerX, int centerY, 
    int innerRadius, int outerRadius, 
    float startAngleRad, float endAngleRad, 
    const std::vector<SDL_Color>& palette,
    Uint8 a);
    void drawAnnularSection(int centerX, int centerY, int innerRadius, int outerRadius, float startAngleRad, float endAngleRad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawTexture(SDL_Texture* texture, int x, int y, double angleRad, int pivotX, int pivotY, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void adjustCameraPosition(Galaxy& currentGalaxy);
    void drawPlanet(Galaxy& currentGalaxy, size_t planetIndex);
    void drawPlanetPlatform(Galaxy& currentGalaxy, size_t platformIndex);
    void drawHuman(Galaxy& currentGalaxy, size_t humanIndex);
    void drawGalaxy(Galaxy& galaxy);

    void present();
};