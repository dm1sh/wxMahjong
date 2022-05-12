#ifndef DRAWER_H
#define DRAWER_H

#include "wxw.h"

#include "utils.h"

#define TILE_HEIGHT 8
#define TILE_WIDTH 6

#define TILE_IMAGES_N 42

class Drawer {
public:
    Drawer();

    void drawTable(wxDC& dc);

    wxSize tableSize;

    wxSize tilePixelSize; // кратно 600x800
    wxSize resolution;
    Dimensions gridSize;
    wxRect tablePixelRect;

    void setBG(const wxSize& tableSize);
    void initScreen(const TLVec& layout);

    wxPoint toGrid(const wxPoint& point);
    wxPoint fromGrid(int x, int y);
    wxPoint fromGrid(const wxPoint& point);

    ThreePoint marked;

private:
    void drawScreen(wxDC& dc);
    void drawTile(wxDC& dc, int8_t index, const wxPoint& position, uint8_t zIndex);
  
    wxImage tileImages[TILE_IMAGES_N];

    wxBitmap bgBitmap;
    wxBitmap screenBitmap;

    bool isBgReady = false;
    bool isScreenReady = false;
};

#endif
