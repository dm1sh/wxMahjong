#include "Controller.h"
#include "utils.h"
#include <wx/xml/xml.h>

Controller::Controller(Drawer& drawer): drawer(drawer) {};

void Controller::resize(const wxSize& tableSize) {
    drawer.setBG(tableSize);
    drawer.initScreen(tableSize);

    resolution = tableSize;

    wxLogDebug("Resize");
}

wxPoint Controller::toGrid(const wxPoint& point) {
    wxPoint out(-1, -1);

    if (point.x >= pixelTableRect.x &&
        point.x <= pixelTableRect.x + pixelTableRect.width &&
        point.y >= pixelTableRect.y &&
        point.y <= pixelTableRect.y + pixelTableRect.height)
    {
        out.x = upDiv(point.x - pixelTableRect.x, tileSize.x);
        out.y = upDiv(point.y - pixelTableRect.y, tileSize.y);
    }

    return out;
}