#include "./Graphics.h"

void Graphics::drawTable(wxDC& dc, wxSize wndSize) {
    drawBG(dc, wndSize);
}

void Graphics::drawBG(wxDC& dc, wxSize wndSize) {
    dc.GradientFillConcentric(wxRect(wxPoint(0, 0), wndSize), wxColor(7, 85, 45), wxColor(1, 45, 22));
}
