#include <SDL.h>
#include "headers/EventListener.h"
#include "headers/Draw.h"
#include "headers/globals.h"
#include "headers/Galaxy.h"
#include "headers/Phisics.h"
#include "headers/EntityController.h"
#include "headers/GalaxyEdit.h"
#include <iostream>

Phisics phisics;
EntityController entityController;
GalaxyEdit galaxyEdit;

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
    //planets
    //Position position, Velocity velocity, Acceleration acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<Radius> planetLayers
    galaxy.addPlanet({-2100, 0}, {0, 0}, {0, 0}, {2000}, {20000}, {0.80f}, {0.0f}, {{1950}, {1750}, {1700}, {1500}, {1450}, {1250}, {1200}, {1000}, {950}, {750}, {700}, {500}, {450}, {250}, {200}, {100}});


    galaxy.addPlanet({800, 0}, {0, 0}, {0, 0}, {300}, {800}, {0.80f}, {0.0f}, {{}});
    galaxy.addPlanet({700, 1100}, {0, 0}, {0, 0}, {300}, {800}, {0.80f}, {0.0f}, {{}});
    galaxy.addPlanet({2000, 700}, {0, 0}, {0, 0}, {300}, {800}, {0.80f}, {0.0f}, {{}});
    galaxy.addPlanet({1900, -600}, {0, 0}, {0, 0}, {300}, {800}, {0.80f}, {0.0f}, {{}});
    galaxy.addPlanet({1000, -2600}, {0, 0}, {0, 0}, {700}, {2500}, {0.80f}, {0.0f}, {{}});
    galaxy.addPlanet({-1300, -5000}, {0, 0}, {0, 0}, {1200}, {5000}, {0.80f}, {0.0f}, {{}});
    //platforms
    //Size size, size_t planetIndex, Angle angle, float angularSpeed
    galaxy.addPlanetPlatform({10, 10}, 7, {0.0f}, 0.3f);
    galaxy.addPlanetPlatform({10, 10}, 1, {30.0f}, 0.3f);
    //walls
    //size_t planetIndex, int planetStartLayer, int planetEndLayer, int width, Angle angle
    galaxy.addPlanetWall(1, 14, 2, 5, {0.0f});
    galaxy.addPlanetWall(1, 14, 5, 5, {30.0f});
    galaxy.addPlanetWall(1, 1, 0, 5, {30.0f});
    galaxy.addPlanetWall(1, 14, 0, 5, {60.0f});
    galaxy.addPlanetWall(1, 14, 0, 5, {90.0f});
    galaxy.addPlanetWall(1, 14, 0, 5, {120.0f});
    galaxy.addPlanetWall(1, 14, 0, 5, {150.0f});
    galaxy.addPlanetWall(1, 14, 0, 5, {180.0f});
    galaxy.addPlanetWall(1, 14, 0, 5, {210.0f});
    galaxy.addPlanetWall(1, 14, 0, 5, {240.0f});
    galaxy.addPlanetWall(1, 14, 0, 5, {270.0f});
    galaxy.addPlanetWall(1, 14, 0, 5, {300.0f});
    galaxy.addPlanetWall(1, 14, 7, 5, {330.0f});
    galaxy.addPlanetWall(1, 5, 4, 5, {330.0f});

    //entries
    //size_t planetIndex, int planetStartLayer, int width, Angle angle
    galaxy.addPlanetEntry(1, 0, 10, {15.0f});
    galaxy.addPlanetEntry(1, 2, 10, {-15.0f});
    galaxy.addPlanetEntry(1, 4, 10, {-45.0f});

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

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    std::cout << "Renderer: " << info.name << std::endl;

    if (!renderer) {
        printf("Errore nella creazione del renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

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