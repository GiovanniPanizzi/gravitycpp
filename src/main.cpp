#include "../include/classes/generalPurpose/Window.hpp"
#include "../include/classes/generalPurpose/EventListener.hpp"
#include "../include/classes/generalPurpose/Draw.hpp"
#include "../include/utilities/globals.hpp"
#include <iostream>

//create main classes
Window window("Gravity 4.0", 800, 600);
EventListener eventListener(window);
Draw draw(window);

void mainPlayingFunction(){

    //draw
    draw.clearScreen(0, 0, 0, 255);
    draw.present();
}

int main(){
    
    while(running){
        eventListener.listenEvents();
        mainPlayingFunction();
    }

    return 0;   
}