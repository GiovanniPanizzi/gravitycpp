#pragma once
#include "globals.h"
#include "stats.h"
#include <vector>
#include "EventListener.h"
#include "Galaxy.h"
#include "Collider.h"
#include <iostream>

class GalaxyEdit {
    private:
    int selectedTool;
    std::vector<Size> toolSizes;
    std::vector<Vec2> toolPositions;
    bool zooming;
    bool dragging;
    bool leftMouseClicked;
    int mouseX;
    int mouseY;
    int mouseStartX;
    int mouseStartY;
    int cameraStartX;
    int cameraStartY;
    Vec2 objectStartPosition;
    bool isSlidingGalaxy;
    Collider collider;

    int dragged;
    bool draggingPlanet;
    bool draggingEntity;
    bool draggingPlatform;

    public:
    GalaxyEdit();
    void mouseClick(Galaxy& currentGalaxy);
    void mouseRelease();
    void getListenerStats(EventListener& eventListener, Galaxy& currentGalaxy);
    void slidingGalaxy(Galaxy& currentGalaxy);
    void moveObject(Galaxy& currentGalaxy);
    void drawEditingTools(Draw& draw);
    void update(Galaxy& currentGalaxy, EventListener& eventListener, Draw& draw);
};