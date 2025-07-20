#pragma once
#include "Window.hpp"
#include "../../utilities/globals.hpp"
#include "../../objects/Galaxy.hpp"

class Draw {
private:
    Window& window;
    GLuint simplePlanetShaderProgram;
    GLuint rectShaderProgram;
    GLuint rectVAO;
    GLuint rectVBO;

public:
    Draw(Window& window);
    virtual ~Draw();
    void drawFilledRotatedRect(float x, float y, float width, float height,
                                  float angleRad, float pivotX, float pivotY,
                                  Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawFilledCircle(float x, float y, float radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawPlanet(Galaxy& currentGalaxy, size_t planetIndex);
    void clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void present();
    void adjustCameraPosition(Galaxy& currentGalaxy);
};