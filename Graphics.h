#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "./wxw.h"

class Graphics {
public:
    Graphics() {};
    ~Graphics() {};

    void drawTable(wxDC& dc, wxSize wndSize);
    void drawBG(wxDC& dc, wxSize wndSize);
};

#endif
