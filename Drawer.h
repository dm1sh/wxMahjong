#ifndef DRAWER_H
#define DRAWER_H

#include "wxw.h"

#include "utils.h"

#define TILE_HEIGHT 8
#define TILE_WIDTH 6

#define TILE_IMAGES_N 42

#define TILE_PADDING_SCALE 1.25

class Drawer {
public:
    Drawer();

    void drawTable(wxDC& dc);

    void composeBG();
    void composeBoard(const TLVec& layout, const Dimensions& gridSize);

    void resizeBg(const wxSize& tableSize);
    bool resizeBoard(const TLVec& layout, const Dimensions& gridSize, bool force);

    wxPoint toGrid(wxPoint point) const;
    wxPoint fromGrid(int x, int y) const;
    wxPoint fromGrid(const wxPoint& point) const;

    wxSize composeMinSize(const Dimensions& gridSize);

    wxSize tableSize;

    wxSize tilePixelSize; // кратно 3x4, по умолчанию 600x800
    wxSize resolution;
    wxRect tablePixelRect;

    ThreePoint marked;

private:
    void drawTile(wxDC& dc, int8_t index, const wxPoint& position,
                  uint8_t zIndex) const;

    wxImage tileImages[TILE_IMAGES_N];

    wxBitmap bgBitmap;
    wxBitmap boardBitmap;

    wxPoint boardPadding;
    wxPoint tilePadding;

    int prevGridPoint;
};

#endif
