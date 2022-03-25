#ifndef DRAWER_H
#define DRAWER_H

#include "./wxw.h"

class Drawer {
public:
    Drawer();

    void drawTable(wxDC& dc);
    void drawTimer(wxStaticText* timerText, int time);
    wxStaticText* initTimer(wxWindow* parent);

    wxSize tableSize;

private:
    void drawBG(wxDC& dc);
    void drawTiles(wxDC& dc);

    wxImage tileImages[40];
};

#endif
