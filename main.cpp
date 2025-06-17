#include <SDL.h>
#include "headers/EventListener.h"
#include "headers/Draw.h"
#include "headers/globals.h"
#include "headers/Galaxy.h"
#include "headers/Phisics.h"
#include "headers/EntityController.h"
#include "headers/GalaxyEdit.h"
#include "headers/SpriteHandler.h"
#include "headers/Menu.h"
#include "headers/WormController.h"
#include <iostream>

Phisics phisics;
EntityController entityController;
GalaxyEdit galaxyEdit;
WormController wormController;
SpriteHandler spriteHandler;
std::vector<Vec2> buttonsPositions = {
    {static_cast<float>(screenWidth) / 2 - 200, 300},
    {static_cast<float>(screenWidth) / 2 - 200, 450},
    {static_cast<float>(screenWidth) / 2 - 200, 600}
};
std::vector<Size> buttonsSizes = {
    {400, 100},
    {400, 100},
    {400, 100},
};
std::vector<Color> buttonsColors = {
    {100, 200, 100, 255},
    {100, 100, 200, 255},
    {200, 100, 100, 255}
};

Menu mainMenu(buttonsPositions, buttonsSizes, buttonsColors);

Menu pauseMenu(buttonsPositions, buttonsSizes, buttonsColors);

int state = 0;

void mainMenuFunction(Draw& draw, EventListener& eventListener){
    switch(mainMenu.handleClicks(eventListener)){
        case 0:
            break;
        case 1:
            state = 1;
            break;
        case 2:
            state = 2;
            break;
        case 3:
            state = 0;
            break;
        default:
            break;
    }
    draw.clearScreen(0, 0, 0, 255);
    mainMenu.draw(draw);
    draw.present();
}

void pauseMenuFunction(Draw& draw, EventListener& eventListener){
    switch(pauseMenu.handleClicks(eventListener)){
        case 0:
            break;
        case 1:
            state = 1;
            break;
        case 2:
            state = 2;
            break;
        case 3:
            state = 0;
            break;
        default:
            break;
    }
    draw.clearScreen(0, 0, 0, 255);
    pauseMenu.draw(draw);
    draw.present();
}

void normalPlayingFunction(Draw& draw, Galaxy& currentGalaxy, EventListener& eventListener){
    phisics.update(currentGalaxy);
    currentGalaxy.adjustCameraPosition();
    currentGalaxy.draw(draw);
    entityController.updateControls(currentGalaxy, 0, eventListener.isLeftPressed(), eventListener.isRightPressed(), eventListener.isUpPressed());
    entityController.drawState(currentGalaxy, draw, 0);
    wormController.allMove(currentGalaxy);
    spriteHandler.update(currentGalaxy);
    draw.present();
    if(eventListener.isEscPressed()){
        state = 3;
    }
}

void editingGalaxyFunction(Draw& draw, Galaxy& currentGalaxy, EventListener& eventListener){
    currentGalaxy.draw(draw);
    galaxyEdit.update(currentGalaxy, eventListener, draw);
    draw.present();
    if(eventListener.isEscPressed()){
        state = 3;
    }
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
    galaxy.addPlanet({200, 1400}, {0, 0}, {0, 0}, {150}, {300}, {0.80f}, {0.0f}, {{}});
    galaxy.addPlanet({100, 1200}, {0, 0}, {0, 0}, {150}, {300}, {0.80f}, {0.0f}, {{}});

    //platforms
    //Size size, size_t planetIndex, Angle angle, float angularSpeed
    galaxy.addPlanetPlatform({10, 10}, 7, {0.0f}, 0.01f);
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

    //entities

    //worms
    galaxy.addWorm({500, 0}, {{30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}, {30}});
    galaxy.addWorm({100, 100}, {{30}, {30}, {50}, {40}, {30}, {20}, {15}, {10}, {5}, {3}, {2}, {1}});

    Galaxy& currentGalaxy = galaxy;

    //SDL initialize
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Errore nell'inizializzazione di SDL2: %s\n", SDL_GetError());
        return -1;
    }
    //create Window
    SDL_Window *window = SDL_CreateWindow("Gravity", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

    if (!renderer) {
        printf("Errore nella creazione del renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    //event listener
    EventListener eventListener(renderer);

    //draw
    Draw draw(*renderer);

    //create sprites
    spriteHandler.createSprite("sprites/spritesheet.png", renderer, 20, 40);

    //timing variables
    float time = 0.0f;
    int frameCount = 0;
    int currentFPS = 0;
    const int desiredFPS = 90;
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
            std::cout << currentFPS << std::endl;
        }
        state1 = state;
        eventListener.listenEvents();
        if(state == 0){
            mainMenuFunction(draw, eventListener);
        }
        if(state == 1){
            normalPlayingFunction(draw, currentGalaxy, eventListener);
        }

        if(state == 2){
            editingGalaxyFunction(draw, currentGalaxy, eventListener);
        }

        if(state == 3){
            pauseMenuFunction(draw, eventListener);
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