#include "../include/classes/generalPurpose/Window.hpp"
#include "../include/classes/generalPurpose/EventListener.hpp"
#include "../include/classes/generalPurpose/Draw.hpp"
#include "../include/classes/systems/Phisics.hpp"
#include "../include/utilities/globals.hpp"
#include "../include/objects/Galaxy.hpp"
#include "../include/classes/systems/HumanController.hpp"
#include <iostream>

//create main classes
Window window("Gravity 5.0", 800, 600);
EventListener eventListener(window);
Draw draw(window);
Phisics phisics;
HumanController humanController;
int i = 0;

void mainPlayingFunction(Galaxy& currentGalaxy){
    //collider
    phisics.update(currentGalaxy);
    humanController.updateControls(currentGalaxy, 0, eventListener.isLeftPressed(), eventListener.isRightPressed(), eventListener.isUpPressed());
    //draw
    draw.clearScreen(5, 5, 1, 255);
    draw.adjustCameraPosition(currentGalaxy);
    //draw.drawGalaxy(currentGalaxy);
    //humanController.drawState(currentGalaxy, draw, 0);

    draw.drawFilledRotatedRect(300.0f, 300.0f, 400.0f, 400.0f, i * 0.01, 200.0f, 200.0f, 150, 100, 50, 255);
    i++;
    draw.present();
}

int main(){

    Galaxy currentGalaxy;

    //create planet
    std::vector<LayerSection> planetLayers;
    float layerRadius = 3000.0f;
    int i = 0;

    while(true){
        if(layerRadius <= 300.0f) break;
        LayerSection layerSection;
        layerSection.material = Material::ROCK;
        layerSection.shape.outerRadius.value = layerRadius;
        if(i % 2 == 0){
            layerSection.shape.startAngle.rad = 0.0f + M_PI * i / 8;
            layerSection.shape.innerRadius.value = layerRadius - 200.0f;
            layerSection.shape.endAngle.rad = 2 * M_PI - M_PI / 8 + M_PI * i / 8;
            layerRadius -= 200.0f;
            while(layerSection.shape.startAngle.rad > 2 * M_PI) {
                layerSection.shape.endAngle.rad -= 2 * M_PI;
                layerSection.shape.startAngle.rad -= 2 * M_PI;
            }
        } else {
            layerSection.shape.startAngle.rad = M_PI / 8 + i * M_PI / 2;
            layerSection.shape.innerRadius.value = layerRadius - 300.0f;
            layerSection.shape.endAngle.rad = 2 * M_PI / 8 + i * M_PI / 2;
            layerRadius -= 300.0f;
        }
        planetLayers.push_back(layerSection);
        i++;
    }
    planetLayers.push_back({{Radius{0.0f}, Radius{200.0f}, Angle{0.0f}, Angle{2 * M_PI}}, Material::GRAVITANIUM});

    currentGalaxy.addPlanet({-2000.0f, 5000.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {3000.0f}, {18000.0f}, {0.9f}, {1.0f}, planetLayers);
    
    currentGalaxy.addPlanet({-100.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {100.0f}, {150.0f}, {0.85f}, {0.85f}, {});

    currentGalaxy.addPlanet({300.0f, 300.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {100.0f}, {150.0f}, {0.85f}, {0.85f}, {});

    currentGalaxy.addPlanet({700.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {100.0f}, {150.0f}, {0.85f}, {0.85f}, {});

    currentGalaxy.addPlanet({300.0f, -1300.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {1000.0f}, {8000.0f}, {0.85f}, {0.85f}, {});

    currentGalaxy.addPlanet({0.0f, 800.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {100.0f}, {150.0f}, {0.85f}, {0.85f}, {});

    currentGalaxy.addPlanet({-400.0f, 1300.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {100.0f}, {150.0f}, {0.85f}, {0.85f}, {});

    //platforms
    AnnularSection platformShape;
    size_t planetIndex = 4;
    float angularSpeed = 0.01f;

    platformShape.outerRadius = {1005.0f};
    platformShape.innerRadius = {1000.0f};
    platformShape.startAngle = {0.0f};
    platformShape.endAngle = {0.3f};

    currentGalaxy.addPlanetPlatform(platformShape, planetIndex, angularSpeed);

    //timing variables
    float time = 0.0f;
    int frameCount = 0;
    int currentFPS = 0;
    const int desiredFPS = 60;
    const int frameDelay = 1000 / desiredFPS;
    Uint32 lastTick = SDL_GetTicks();
    float deltaTime;
    
    while(running){
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
    
        mainPlayingFunction(currentGalaxy);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;   
}