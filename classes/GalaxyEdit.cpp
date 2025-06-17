#include "../headers/GalaxyEdit.h"

bool pointInRect(float x, float y, Vec2 rectPosition, Size rectSize){
    return (
        x >= rectPosition.x &&
        x <= rectPosition.x + rectSize.width &&
        y >= rectPosition.y &&
        y <= rectPosition.y + rectSize.height
    );
}

GalaxyEdit::GalaxyEdit(){
    //moving tool 0
    selectedTool = 0;
    toolSizes.push_back(Size{50, 30});
    toolPositions.push_back(Vec2{20, 20});
    //adding tool 1
    toolSizes.push_back(Size{50, 30});
    toolPositions.push_back(Vec2{70, 20});
    //rubber tool 2
    toolSizes.push_back(Size{50, 30});
    toolPositions.push_back(Vec2{120, 20});
    // zooming bar 3
    toolSizes.push_back(Size{150, 20});
    toolPositions.push_back(Vec2{static_cast<float>(screenWidth) / 2 - 75, static_cast<float>(screenHeight) - 40});

    zooming = false;
    dragging = false;
    leftMouseClicked = false;
    isSlidingGalaxy = false;

    dragged = -1;
    draggingPlanet = false;
    draggingEntity = false;
    draggingPlatform = false;
}

void GalaxyEdit::mouseClick(Galaxy& currentGalaxy) {
    leftMouseClicked = true;
    for (int i = 0; i < toolSizes.size(); i++) {
        if (pointInRect(mouseX, mouseY, toolPositions[i], toolSizes[i])) {
            selectedTool = i;
            return;
        }
    }

    if (selectedTool == 0) { // Moving tool
        for (size_t i = 0; i < currentGalaxy.planets.entities.size(); i++) {
            size_t planetIndex = currentGalaxy.planets.entities[i].index;
            if (collider.pointInCircle(
                    mouseX, mouseY,
                    currentGalaxy.planets.positions[planetIndex].x - cameraPosition.x + screenWidth / 2,
                    currentGalaxy.planets.positions[planetIndex].y - cameraPosition.y + screenHeight / 2,
                    currentGalaxy.planets.radii[planetIndex].value)) {
                dragged = planetIndex;
                objectStartPosition = currentGalaxy.planets.positions[dragged];
                draggingPlanet = true;
                dragging = true;
                return;
            }
        }

        for (size_t i = 0; i < currentGalaxy.planets.planetPlatforms.size(); i++) {
            for (size_t j = 0; j < currentGalaxy.planets.planetPlatforms[i].size(); j++) {
                size_t platformIndex = currentGalaxy.planets.planetPlatforms[i][j];
                size_t planetId = i;

                float planetX = currentGalaxy.planets.positions[planetId].x;
                float planetY = currentGalaxy.planets.positions[planetId].y;
                float planetR = currentGalaxy.planets.radii[planetId].value;

                float angleDeg = currentGalaxy.platforms.angles[platformIndex].deg;
                float height = currentGalaxy.platforms.sizes[platformIndex].height;
                float width = currentGalaxy.platforms.sizes[platformIndex].width;

                float worldMouseX = mouseX / scale + cameraPosition.x - screenWidth / 2;
                float worldMouseY = mouseY / scale + cameraPosition.y - screenHeight / 2;

                if (collider.pointInPlatform(worldMouseX, worldMouseY, planetX, planetY, planetR, height, width, angleDeg)) {
                    dragged = platformIndex;
                    draggingPlatform = true;
                    dragging = true;
                    return;
                }
            }
        }
    }

    if (selectedTool == 2) { // Rubber tool
        for (size_t i = 0; i < currentGalaxy.planets.entities.size(); i++) {
            size_t planetIndex = currentGalaxy.planets.entities[i].index;
            if (collider.pointInCircle(
                    mouseX, mouseY,
                    currentGalaxy.planets.positions[planetIndex].x - cameraPosition.x + screenWidth / 2,
                    currentGalaxy.planets.positions[planetIndex].y - cameraPosition.y + screenHeight / 2,
                    currentGalaxy.planets.radii[planetIndex].value)) {
                currentGalaxy.removePlanet(planetIndex);
                return;
            }
        }

        for (size_t i = 0; i < currentGalaxy.planets.planetPlatforms.size(); i++) {
            for (size_t j = 0; j < currentGalaxy.planets.planetPlatforms[i].size(); j++) {
                size_t platformIndex = currentGalaxy.planets.planetPlatforms[i][j];
                size_t planetId = i;

                float planetX = currentGalaxy.planets.positions[planetId].x;
                float planetY = currentGalaxy.planets.positions[planetId].y;
                float planetR = currentGalaxy.planets.radii[planetId].value;

                float angleDeg = currentGalaxy.platforms.angles[platformIndex].deg;
                float height = currentGalaxy.platforms.sizes[platformIndex].height;
                float width = currentGalaxy.platforms.sizes[platformIndex].width;

                float worldMouseX = mouseX / scale + cameraPosition.x - screenWidth / 2;
                float worldMouseY = mouseY / scale + cameraPosition.y - screenHeight / 2;

                if (collider.pointInPlatform(worldMouseX, worldMouseY, planetX, planetY, planetR, height, width, angleDeg)) {
                    currentGalaxy.removePlanetPlatform(platformIndex);
                    return;
                }
            }
        }
    }

    isSlidingGalaxy = true;
    dragging = true;
}

void GalaxyEdit::moveObject(Galaxy& currentGalaxy) {
    if (draggingPlanet && dragged != -1) {
        // Move planet with the mouse (world coordinates)
        float worldMouseX = mouseX / scale + cameraPosition.x - screenWidth / 2;
        float worldMouseY = mouseY / scale + cameraPosition.y - screenHeight / 2;
        float worldMouseStartX = mouseStartX / scale + cameraPosition.x - screenWidth / 2;
        float worldMouseStartY = mouseStartY / scale + cameraPosition.y - screenHeight / 2;

        currentGalaxy.planets.positions[dragged].x = objectStartPosition.x + (worldMouseX - worldMouseStartX);
        currentGalaxy.planets.positions[dragged].y = objectStartPosition.y + (worldMouseY - worldMouseStartY);
    } else if (draggingPlatform && dragged != -1) {
        // Rotate platform around its associated planet
        Vec2& planetPos = currentGalaxy.planets.positions[dragged];

        float worldMouseX = mouseX / scale + cameraPosition.x - screenWidth / 2;
        float worldMouseY = mouseY / scale + cameraPosition.y - screenHeight / 2;

        float dx = worldMouseX - planetPos.x;
        float dy = worldMouseY - planetPos.y;

        float angle = atan2(dy, dx) * 180.0f / M_PI;

        currentGalaxy.planets.angles[dragged].deg = angle;
    }
}

void GalaxyEdit::mouseRelease() {
    isSlidingGalaxy = false;
    zooming = false;
    dragging = false;
    leftMouseClicked = false;
    dragged = -1;
    draggingPlanet = false;
    draggingEntity = false;
    draggingPlatform = false;
}

void GalaxyEdit::slidingGalaxy(Galaxy& currentGalaxy){
    cameraPosition.x = cameraStartX - mouseX / scale + mouseStartX / scale;
    cameraPosition.y = cameraStartY - mouseY / scale + mouseStartY / scale;
}

void GalaxyEdit::getListenerStats(EventListener& eventListener, Galaxy& currentGalaxy){
    if(eventListener.isLeftMousePressed()){
        if(!leftMouseClicked){ 
            cameraStartX = cameraPosition.x;
            cameraStartY = cameraPosition.y;
            mouseStartX = mouseX = eventListener.getMouseX();
            mouseStartY = mouseY = eventListener.getMouseY();
            mouseClick(currentGalaxy);
        }
        if(dragging){
            mouseX = eventListener.getMouseX();
            mouseY = eventListener.getMouseY();
        }
    }
    else{
        if(leftMouseClicked) mouseRelease();
    }
}

void GalaxyEdit::drawEditingTools(Draw& draw){
    //moving tool
    draw.drawRect(toolPositions[0].x, toolPositions[0].y, toolSizes[0].width, toolSizes[0].height, 0, 0, 0, 255);
    //adding tool
    draw.drawRect(toolPositions[1].x, toolPositions[1].y, toolSizes[1].width, toolSizes[1].height, 100, 100, 100, 255);
    //rubber tool
    draw.drawRect(toolPositions[2].x, toolPositions[2].y, toolSizes[2].width, toolSizes[2].height, 255, 255, 255, 255);

    //zooming bar
    draw.drawRect(toolPositions[3].x, toolPositions[3].y, toolSizes[3].width, toolSizes[3].height, 255, 255, 255, 255);
}

void GalaxyEdit::update(Galaxy& currentGalaxy, EventListener& eventListener, Draw& draw){
    getListenerStats(eventListener, currentGalaxy);
    if(isSlidingGalaxy) slidingGalaxy(currentGalaxy);
    if(dragged != -1) moveObject(currentGalaxy);
    drawEditingTools(draw);
}