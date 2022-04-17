#include "Drawer.h"

#define TILE_IMAGES_N 37
static const char* tileImageNames[] = { "Back", "Blank", "Chun", "Front", "Haku", "Hatsu", "Man1", "Man2", "Man3", "Man4", "Man5", "Man6", "Man7", "Man8", "Man9", "Nan", "Pei", "Pin1", "Pin2", "Pin3", "Pin4", "Pin5", "Pin6", "Pin7", "Pin8", "Pin9", "Shaa", "Sou1", "Sou2", "Sou3", "Sou4", "Sou5", "Sou6", "Sou7", "Sou8", "Sou9", "Ton" };

Drawer::Drawer() {
    for (int i = 0; i < 37; i++) {
        tileImages[i].LoadFile(_("./resources/tiles/") + _(tileImageNames[i]) + _(".png"), wxBITMAP_TYPE_PNG);
        tileImages[i].Rescale(60, 80);
    }
}

void Drawer::drawTable(wxDC& dc) {
    wxLogDebug("Redraw");

    if  (isScreenReady)
        dc.DrawBitmap(screenBitmap, 0, 0, false);
}

wxBitmap copyBitmap(const wxBitmap& old) {
    return old.GetSubBitmap(wxRect(0, 0, old.GetWidth(), old.GetHeight()));
}

void Drawer::setBG(const wxSize& tableSize) {
    wxLogDebug("Recreate background");

    bgBitmap = wxBitmap(tableSize);

    wxMemoryDC dc;
    dc.SelectObject(bgBitmap);

    dc.GradientFillConcentric(wxRect(wxPoint(0, 0), tableSize), wxColor(7, 85, 45), wxColor(1, 45, 22));

    isBgReady = true;
}

void Drawer::initScreen(const wxSize& tableSize) {
    wxLogDebug("Recreate screen");
    if (isBgReady) {
        screenBitmap = copyBitmap(bgBitmap);
        isScreenReady = true;
    }
}

void Drawer::drawTile(wxDC& dc, const char index) {
    dc.DrawBitmap(tileImages[3], wxPoint(10, 10));
    dc.DrawBitmap(tileImages[index], wxPoint(10, 10));
}
