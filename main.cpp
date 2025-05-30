#include <SDL.h>
#include "headers/EventListener.h"
#include "headers/Draw.h"
#include "headers/globals.h"
#include "headers/Galaxy.h"
#include "headers/Phisics.h"
#include "headers/EntityController.h"
#include "headers/GalaxyEdit.h"
#include "headers/TextureLoader.h"
#include "headers/SpriteHandler.h"
#include <iostream>

Phisics phisics;
EntityController entityController;
GalaxyEdit galaxyEdit;
TextureLoader textureLoader;
SpriteHandler spriteHandler;

int state = 1;

void normalPlayingFunction(Draw& draw, Galaxy& currentGalaxy, EventListener& eventListener){
    if(eventListener.isLeftPressed()){
        entityController.moveLeft(currentGalaxy, 0);
    }
    if(eventListener.isRightPressed()){
        entityController.moveRight(currentGalaxy, 0);
    }
    if(eventListener.isUpPressed()){
        entityController.jump(currentGalaxy, 0);
    }
    phisics.update(currentGalaxy);
    spriteHandler.update(currentGalaxy);
    currentGalaxy.draw(draw);
    draw.present();
}

void editingGalaxyFunction(Draw& draw, Galaxy& currentGalaxy, EventListener& eventListener){
    
    currentGalaxy.draw(draw);
    galaxyEdit.update(currentGalaxy, eventListener, draw);
    draw.present();
}

int main(int argc, char *argv[]) {
    Galaxy galaxy;

    //planet
    Position pos = {-100.0f, -100.0f};
    Velocity vel = {0.0f, 0.0f};
    Acceleration acc = {0.0f, 0.0f};
    Radius radius = {200.0f};
    Mass mass = {700.0f};
    Friction friction = {0.9f};
    Elasticity elasticity = {0.0f};
    galaxy.addPlanet(pos, vel, acc, radius, mass, friction, elasticity);

    //platform
    Size size = {100, 10};
    Angle angle = {0.0f};
    float angularSpeed = 1.0f;
    galaxy.addPlanetPlatform(size, 1, angle, angularSpeed);

    //hollowPlanet
    pos = {0.0f, 1300.0f};
    vel = {0.0f, 0.0f};
    acc = {0.0f, 0.0f};
    radius = {1000.0f};
    mass = {10000.0f};
    friction = {0.9f};
    elasticity = {0.0f};
    std::vector<Radius> hollowRadii = {{950.0f}, {750.0f}, {700.0f}, {550.0f}, {500.0f}, {400.0f}};
    std::vector<layerEntry> layerEntries = {{0, {0}, 20.0f}, 
    {2, {90}, 20.0f}, 
    {4, {0}, 20.0f}};
    std::vector<layerEntry> walls = {{1, {-30}, 40.0f}, {3, {-90}, 20,}};
    galaxy.addHollowPlanet(pos, vel, acc, radius, mass, friction, elasticity, hollowRadii, layerEntries, walls);

    Galaxy& currentGalaxy = galaxy;

    //SDL initialize
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Errore nell'inizializzazione di SDL2: %s\n", SDL_GetError());
        return -1;
    }
    //create Window
    SDL_Window *window = SDL_CreateWindow("Gravity", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Errore nella creazione della finestra: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    //renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (!renderer) {
        printf("Errore nella creazione del renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    spriteHandler.createSprite("sprites/spriteSheet.png", renderer, 64, 128);

    //event listener
    EventListener eventListener;

    //draw
    Draw draw(*renderer);

    //timing variables
    float time = 0.0f;
    int frameCount = 0;
    int currentFPS = 0;
    const int desiredFPS = 60;
    const int frameDelay = 1000 / desiredFPS;
    Uint32 lastTick = SDL_GetTicks();
    float deltaTime;

    //main loop
    while (running) {
        Uint32 frameStart = SDL_GetTicks();
        deltaTime = (frameStart - lastTick) / 1000.0f;
        lastTick = frameStart;

        time += deltaTime;
        frameCount++;

        if (time >= 1.0f) {
            currentFPS = frameCount;
            frameCount = 0;
            time = 0.0f;
            //std::cout << currentFPS << std::endl;
        }
        eventListener.listenEvents();

        if(state == 1){
            currentGalaxy.sizes[0] = {20, 40};
            normalPlayingFunction(draw, currentGalaxy, eventListener);
        }

        if(state == 2){
            currentGalaxy.sizes[0] = {1, 1};
            editingGalaxyFunction(draw, currentGalaxy, eventListener);
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }


    //destroy 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}