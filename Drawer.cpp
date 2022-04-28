#include "Drawer.h"

#define TILE_IMAGES_N 37
static const char* tileImageNames[] = { "Back", "Blank", "Chun", "Front", "Haku", "Hatsu", "Man1", "Man2", "Man3", "Man4", "Man5", "Man6", "Man7", "Man8", "Man9", "Nan", "Pei", "Pin1", "Pin2", "Pin3", "Pin4", "Pin5", "Pin6", "Pin7", "Pin8", "Pin9", "Shaa", "Sou1", "Sou2", "Sou3", "Sou4", "Sou5", "Sou6", "Sou7", "Sou8", "Sou9", "Ton" };

Drawer::Drawer() {
    for (int i = 0; i < 37; i++)
        tileImages[i].LoadFile(_("./resources/tiles/") + _(tileImageNames[i]) + _(".png"), wxBITMAP_TYPE_PNG);
}

void Drawer::drawTable(wxDC& dc) {
    if (isScreenReady)
        dc.DrawBitmap(screenBitmap, 0, 0, false);
}

wxBitmap copyBitmap(const wxBitmap& old) {
    return old.GetSubBitmap(wxRect(0, 0, old.GetWidth(), old.GetHeight()));
}

void Drawer::setBG(const wxSize& tableSize) {
    bgBitmap = wxBitmap(tableSize);

    wxMemoryDC dc;
    dc.SelectObject(bgBitmap);

    dc.GradientFillConcentric(wxRect(wxPoint(0, 0), tableSize), wxColor(7, 85, 45), wxColor(1, 45, 22));

    isBgReady = true;
}

void Drawer::initScreen(const wxSize& tableSize, const TLVec& layout) {
    if (isBgReady) {
        screenBitmap = copyBitmap(bgBitmap);

        wxMemoryDC dc;
        dc.SelectObject(screenBitmap);

        int cards_set = -1;
        uint8_t layer = -1;

        while (cards_set) {
            layer++;
            cards_set = 0;

            for (int i = 0; i < layout.size(); i++)
                for (int j = 0; j < layout[i].size(); j++) {
                    int k = layout[i][j].size() - 1;
                    while (k > -1 && layout[i][j][k].second == (uint8_t)-1)
                        k--;

                    if (k > -1)
                        if (layout[i][j][k].first == layer) {
                            drawTile(dc, layout[i][j][k].second, {tablePixelRect.x + tilePixelSize.x*i, tablePixelRect.y + tilePixelSize.y*j});
                            cards_set++;
                        }
                    }
        }

        isScreenReady = true;
    }
}

void Drawer::drawTile (wxDC& dc, int8_t index, const wxPoint& position) {
    dc.DrawRoundedRectangle(position, tilePixelSize * 2, 10);

    if (tileImages[index].GetWidth() != tilePixelSize.x * 2)
        dc.DrawBitmap(tileImages[index].Scale(tilePixelSize.x * 2 - 20, tilePixelSize.y * 2 - 20), {position.x + 10, position.y + 10});
    else
        dc.DrawBitmap(tileImages[index], position);
}
