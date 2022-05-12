#include "Drawer.h"

static const char* tileImageNames[] = { "Pin1", "Pin2", "Pin3", "Pin4", "Pin5", "Pin6", "Pin7", "Pin8", "Pin9", "Sou1", "Sou2", "Sou3", "Sou4", "Sou5", "Sou6", "Sou7", "Sou8", "Sou9", "Man1", "Man2", "Man3", "Man4", "Man5", "Man6", "Man7", "Man8", "Man9", "Chun", "Haku", "Hatsu", "Nan", "Pei", "Shaa", "Ton", "Flower1", "Flower2", "Flower3", "Flower4", "Season1", "Season2", "Season3", "Season4" };

Drawer::Drawer():  marked{-1, -1, -1} {
    for (int i = 0; i < TILE_IMAGES_N; i++) {
        if (!tileImages[i].LoadFile(_("./resources/tiles/") + _(tileImageNames[i]) + _(".png"), wxBITMAP_TYPE_PNG))
            wxLogDebug(_("./resources/tiles/") + _(tileImageNames[i]) + _(".png ") + wxString::Format(wxT("%i"), i));
    }
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

wxPoint Drawer::toGrid(const wxPoint& point) {
    wxPoint out(-1, -1);

    if (point.x >= tablePixelRect.x &&
        point.x <= tablePixelRect.x + tablePixelRect.width &&
        point.y >= tablePixelRect.y &&
        point.y <= tablePixelRect.y + tablePixelRect.height)
    {
        out.x = (point.x - tablePixelRect.x) / tilePixelSize.x;
        out.y = (point.y - tablePixelRect.y) / tilePixelSize.y;
    }

    return out;
}

wxPoint Drawer::fromGrid(int x, int y) {
    return { tablePixelRect.x + x * tilePixelSize.x,
             tablePixelRect.y + y * tilePixelSize.y };
}

wxPoint Drawer::fromGrid(const wxPoint& point) {
    return fromGrid(point.x, point.y);
}

void Drawer::initScreen(const TLVec& layout) {
    if (isBgReady) {
        screenBitmap = copyBitmap(bgBitmap);

        wxLogDebug(_("Reinit"));

        wxMemoryDC dc;
        dc.SelectObject(screenBitmap);

        for (int z = 0; z < gridSize.z; z++)
            for (int x = 0; x < gridSize.x; x++)
                for (int y = 0; y < gridSize.y; y++) {
                    CardT c = layout[z][x][y];
                    if (c >= 0)
                        drawTile(dc, c, fromGrid(x, y), z);
                }

        isScreenReady = true;
    }
}

void Drawer::drawTile(wxDC& dc, int8_t index, const wxPoint& position, uint8_t zIndex) {
    wxBrush _bgColor = dc.GetBrush();

    wxBrush front = wxColor(255, 255, 255);
    wxBrush back = wxColor(200, 200, 200);

    if (position == fromGrid({marked.x, marked.y}) && marked.z == zIndex) {
        front = wxColor(200, 255, 200);
        back = wxColor(190, 220, 190);
    }

    dc.SetBrush(back);

    dc.DrawRoundedRectangle(position.x + (tilePixelSize.GetWidth()/10 + 3) - (tilePixelSize.GetWidth()/10 + 3)*zIndex, position.y + (tilePixelSize.GetHeight()/10 + 3) - (tilePixelSize.GetHeight()/10 + 3)*zIndex, tilePixelSize.GetWidth() * 2, tilePixelSize.GetHeight() * 2, 10);

    dc.SetBrush(front);

    dc.DrawRoundedRectangle(position.x - (tilePixelSize.GetWidth()/10 + 3)*zIndex, position.y - (tilePixelSize.GetHeight()/10 + 3)*zIndex, tilePixelSize.GetWidth() * 2, tilePixelSize.GetHeight() * 2, 10);

    dc.SetBrush(_bgColor);

    if (tileImages[index].IsOk()) {
        if (tileImages[index].GetWidth() != tilePixelSize.x * 2)
            dc.DrawBitmap(tileImages[index].Scale(tilePixelSize.x * 2 - 20, tilePixelSize.y * 2 - 20), {position.x + 10 - (tilePixelSize.GetWidth()/10 + 3)*zIndex, position.y + 10 - (tilePixelSize.GetHeight()/10 + 3)*zIndex});
        else
            dc.DrawBitmap(tileImages[index], {position.x + 10 - (tilePixelSize.GetWidth()/10 + 3)*zIndex, position.y + 10 - (tilePixelSize.GetHeight()/10 + 3)*zIndex});
    }
}
