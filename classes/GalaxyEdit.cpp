#include "../headers/GalaxyEdit.h"

bool pointInRect(float x, float y, Position rectPosition, Size rectSize){
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
    toolPositions.push_back(Position{20, 20});
    //adding tool 1
    toolSizes.push_back(Size{50, 30});
    toolPositions.push_back(Position{70, 20});
    //rubber tool 2
    toolSizes.push_back(Size{50, 30});
    toolPositions.push_back(Position{120, 20});
    // zooming bar 3
    toolSizes.push_back(Size{150, 20});
    toolPositions.push_back(Position{static_cast<float>(screenWidth) / 2 - 75, static_cast<float>(screenHeight) - 40});

    zooming = false;
    dragging = false;
    leftMouseClicked = false;
    isSlidingGalaxy = false;

    dragged = -1;
    draggingPlanet = false;
    draggingEntity = false;
    draggingPlatform = false;
}

void GalaxyEdit::mouseClick(Galaxy& currentGalaxy){
    leftMouseClicked = true;
    for(int i = 0; i < toolSizes.size(); i++){
        if(pointInRect(mouseX, mouseY, toolPositions[i], toolSizes[i])){
            selectedTool = i;
            return;
        }
    }
    if(selectedTool == 0){
        for(size_t i = 0; i < currentGalaxy.planets.size(); i++){
            size_t planetIndex = currentGalaxy.planets[i].index;
            if(collider.pointInCircle(mouseX, mouseY, currentGalaxy.positions[planetIndex].x - currentGalaxy.positions[0].x + screenWidth / 2, currentGalaxy.positions[planetIndex].y - currentGalaxy.positions[0].y + screenHeight / 2, currentGalaxy.radii[planetIndex].value)){
                dragged = planetIndex;
                draggingPlanet = true;
                dragging = true;
                return;
            }
        }
        for (size_t i = 0; i < currentGalaxy.platforms.size(); i++) {
            size_t platformIndex = currentGalaxy.platforms[i].index;
            size_t planetId = currentGalaxy.planetIndexes[platformIndex];

            float planetX = currentGalaxy.positions[planetId].x;
            float planetY = currentGalaxy.positions[planetId].y;
            float planetR = currentGalaxy.radii[planetId].value;

            float angleDeg = currentGalaxy.angles[platformIndex].deg;
            float height = currentGalaxy.sizes[platformIndex].height;
            float width = currentGalaxy.sizes[platformIndex].width;     

            float worldMouseX = mouseX / scale + currentGalaxy.positions[0].x - screenWidth / 2;
            float worldMouseY = mouseY / scale + currentGalaxy.positions[0].y - screenHeight / 2;

            if (collider.pointInPlatform(worldMouseX, worldMouseY, planetX, planetY, planetR, height, width, angleDeg)) {
                dragged = platformIndex;
                draggingPlatform = true;
                dragging = true;
                return;
            }
        }
    }
    if(selectedTool == 1){

    }
    if(selectedTool == 2){
        for(size_t i = 0; i < currentGalaxy.planets.size(); i++){
            size_t planetIndex = currentGalaxy.planets[i].index;
            if(collider.pointInCircle(mouseX, mouseY, currentGalaxy.positions[planetIndex].x - currentGalaxy.positions[0].x + screenWidth / 2, currentGalaxy.positions[planetIndex].y - currentGalaxy.positions[0].y + screenHeight / 2, currentGalaxy.radii[planetIndex].value)){
                currentGalaxy.removePlanet(planetIndex);
                return;
            }
        }
        for (size_t i = 0; i < currentGalaxy.platforms.size(); i++) {
            size_t platformIndex = currentGalaxy.platforms[i].index;
            size_t planetId = currentGalaxy.planetIndexes[platformIndex];

            float planetX = currentGalaxy.positions[planetId].x;
            float planetY = currentGalaxy.positions[planetId].y;
            float planetR = currentGalaxy.radii[planetId].value;

            float angleDeg = currentGalaxy.angles[platformIndex].deg;
            float height = currentGalaxy.sizes[platformIndex].height;
            float width = currentGalaxy.sizes[platformIndex].width;     

            float worldMouseX = mouseX / scale + currentGalaxy.positions[0].x - screenWidth / 2;
            float worldMouseY = mouseY / scale + currentGalaxy.positions[0].y - screenHeight / 2;

            if (collider.pointInPlatform(worldMouseX, worldMouseY, planetX, planetY, planetR, height, width, angleDeg)) {
                currentGalaxy.removePlanetPlatform(platformIndex);
                return;
            }
        }
    }

    isSlidingGalaxy = true;
    dragging = true;
}

void GalaxyEdit::moveObject(Galaxy& currentGalaxy) {
    if (draggingPlanet && dragged != -1) {
        // Muovo pianeta con il mouse (coordinate mondo)
        float worldMouseX = mouseX / scale + currentGalaxy.positions[0].x - screenWidth / 2;
        float worldMouseY = mouseY / scale + currentGalaxy.positions[0].y - screenHeight / 2;

        currentGalaxy.positions[dragged].x = worldMouseX;
        currentGalaxy.positions[dragged].y = worldMouseY;
    }
    else if (draggingEntity && dragged != -1) {
        // Muovo entità con il mouse (coordinate mondo)
        float worldMouseX = mouseX / scale + currentGalaxy.positions[0].x - screenWidth / 2;
        float worldMouseY = mouseY / scale + currentGalaxy.positions[0].y - screenHeight / 2;

        currentGalaxy.positions[dragged].x = worldMouseX;
        currentGalaxy.positions[dragged].y = worldMouseY;
    }
    else if (draggingPlatform && dragged != -1) {
        // Ruoto piattaforma attorno al pianeta corrispondente

        // Indice pianeta a cui è legata la piattaforma
        int planetIndex = currentGalaxy.planetIndexes[dragged];

        Position& planetPos = currentGalaxy.positions[planetIndex];

        // Calcolo vettore dal pianeta al mouse (coordinate mondo)
        float worldMouseX = mouseX / scale + currentGalaxy.positions[0].x - screenWidth / 2;
        float worldMouseY = mouseY / scale + currentGalaxy.positions[0].y - screenHeight / 2;

        float dx = worldMouseX - planetPos.x;
        float dy = worldMouseY - planetPos.y;

        // Calcolo angolo con atan2 (in radianti)
        float angle = atan2(dy, dx) * 180.0f / M_PI;

        // Aggiorna l'angolo della piattaforma
        currentGalaxy.angles[dragged].deg = angle;
    }
}

void GalaxyEdit::mouseRelease(){
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
    currentGalaxy.positions[0].x = playerStartX - mouseX / scale + mouseStartX / scale;
    currentGalaxy.positions[0].y = playerStartY - mouseY / scale + mouseStartY / scale;
}

void GalaxyEdit::getListenerStats(EventListener& eventListener, Galaxy& currentGalaxy){
    if(eventListener.isLeftMousePressed()){
        if(!leftMouseClicked){ 
            playerStartX = currentGalaxy.positions[0].x;
            playerStartY = currentGalaxy.positions[0].y;
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