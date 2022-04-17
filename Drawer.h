#ifndef DRAWER_H
#define DRAWER_H

#include "wxw.h"

class Drawer {
public:
    Drawer();

    void drawTable(wxDC& dc);

    wxSize tableSize;

    void setBG(const wxSize& tableSize);
    void initScreen(const wxSize& tableSize);

private:
    void drawScreen(wxDC& dc);
    void drawTile(wxDC& dc, const char index);
  
    wxImage tileImages[40];

    wxBitmap bgBitmap;
    wxBitmap screenBitmap;

    bool isBgReady = false;
    bool isScreenReady = false;
};

#endif
