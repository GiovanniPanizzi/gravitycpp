#include "../include/classes/Window.hpp"
#include "../include/classes/EventListener.hpp"
#include "../include/classes/Draw.hpp"
#include "../include/utilities/globals.hpp"
#include <iostream>

int main(){
    Window window("Gravity 4.0", 800, 600);
    EventListener eventListener(window);
    
    while(running){
        eventListener.listenEvents();
    }

    return 0;   
}