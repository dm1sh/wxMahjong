#ifndef DRAWER_H
#define DRAWER_H

#include "wxw.h"

#include "utils.h"

#define TILE_HEIGHT 8
#define TILE_WIDTH 6

class Drawer {
public:
    Drawer();

    void drawTable(wxDC& dc);

    wxSize tableSize;

    wxSize tilePixelSize; // 600x800
    wxSize resolution;
    wxSize gridSize;
    wxRect tablePixelRect;

    void setBG(const wxSize& tableSize);
    void initScreen(const wxSize& tableSize, const TLVec& layout);

private:
    void drawScreen(wxDC& dc);
    void drawTile(wxDC& dc, int8_t index, const wxPoint& position);
  
    wxImage tileImages[40];

    wxBitmap bgBitmap;
    wxBitmap screenBitmap;

    bool isBgReady = false;
    bool isScreenReady = false;
};

#endif
