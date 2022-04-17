#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "wxw.h"

#include "Drawer.h"

class Controller {
public:
    Controller(Drawer& drawer);

    int stopwatch = 0;

    void resize(const wxSize& tableSize);

    wxPoint toGrid(const wxPoint& point);
private:
    Drawer& drawer;

    wxSize tileSize;
    wxSize resolution;
    wxSize gridSize;
    wxRect pixelTableRect;
};

#endif
