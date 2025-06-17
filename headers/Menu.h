#include "Draw.h"
#include "EventListener.h"
#include "globals.h"
#include "stats.h"
#include "objects.h"

class Menu {
    private:
    std::vector<Vec2> buttonsPositions;
    std::vector<Size> buttonsSizes;
    std::vector<Color> buttonsColors;
    std::vector<Object> buttons;
    public:
    Menu(std::vector<Vec2> positions, std::vector<Size> sizes, std::vector<Color> colors);
    int handleClicks(EventListener& eventListener);
    void handleMouseMovement(EventListener& eventListener);
    void draw(Draw& draw);
};