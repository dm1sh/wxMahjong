#include "Drawer.h"

static const char* tileImageNames[] = {
    // clang-format off
    "Pin1",    "Pin2",    "Pin3",    "Pin4",    "Pin5",    "Pin6",   "Pin7",    "Pin8",    "Pin9",
    "Sou1",    "Sou2",    "Sou3",    "Sou4",    "Sou5",    "Sou6",   "Sou7",    "Sou8",    "Sou9",
    "Man1",    "Man2",    "Man3",    "Man4",    "Man5",    "Man6",   "Man7",    "Man8",    "Man9",
    "Chun",    "Haku",    "Hatsu",
    "Nan",     "Pei",     "Shaa",    "Ton",
    "Flower1", "Flower2", "Flower3", "Flower4",
    "Season1", "Season2", "Season3", "Season4"
    // clang-format on
};

static const wxColor lGreen{0x07, 0x55, 0x2b};
static const wxColor dGreen{0x01, 0x2d, 0x16};

Drawer::Drawer() : marked{-1, -1, -1} {
    for (int i = 0; i < TILE_IMAGES_N; i++) {
        bool succeed = tileImages[i].LoadFile(
            _("./resources/tiles/") + _(tileImageNames[i]) + _(".png"),
            wxBITMAP_TYPE_PNG);
        if (!succeed)
            wxLogDebug(_("failed to load tile ./resources/tiles/") +
                       _(tileImageNames[i]) + _(".png with index") +
                       wxString::Format("%i", i));
    }
}

void Drawer::drawTable(wxDC& dc) {
    dc.DrawBitmap(bgBitmap, 0, 0, false);
    if (boardBitmap.IsOk()) {
        wxLogDebug("Drawing board");
        dc.DrawBitmap(boardBitmap, tablePixelRect.GetPosition(), true);
    }
}

void Drawer::composeBG() {
    bgBitmap = wxBitmap(resolution);

    wxLogDebug(
        wxString::Format("Rebuild bg %i %i", resolution.x, resolution.y));

    wxMemoryDC dc;
    dc.SelectObject(bgBitmap);

    dc.GradientFillConcentric(wxRect(wxPoint(0, 0), resolution), lGreen,
                              dGreen);
}

void Drawer::composeBoard(const TLVec& layout, const Dimensions& gridSize) {
    boardBitmap = wxBitmap(tablePixelRect.GetSize());

    wxLogDebug(_("Rebuild board"));

    wxMemoryDC dc;
    dc.SelectObject(boardBitmap);

    for (int z = 0; z < gridSize.z; z++)
        for (int x = 0; x < gridSize.x; x++)
            for (int y = 0; y < gridSize.y; y++) {
                CardT c = layout[z][x][y];
                if (c >= 0)
                    drawTile(dc, c, fromGrid(x, y), z);
            }

    wxMask* mask = new wxMask(boardBitmap, wxColor(0x00, 0x00, 0x00));
    boardBitmap.SetMask(mask);
}

void Drawer::drawTile(wxDC& dc, int8_t index, const wxPoint& position,
                      uint8_t zIndex) const {
    wxBrush _bgColor = dc.GetBrush();

    wxBrush front = wxColor(0xff, 0xff, 0xff);
    wxBrush back = wxColor(0xc8, 0xc8, 0xc8);

    if (position == fromGrid({marked.x, marked.y}) && marked.z == zIndex) {
        front = wxColor(0xc8, 0xff, 0xc8);
        back = wxColor(0xbe, 0xdc, 0xbe);
    }

    dc.SetBrush(back);

    dc.DrawRoundedRectangle(position.x + (tilePixelSize.GetWidth() / 10 + 3) -
                                (tilePixelSize.GetWidth() / 10 + 3) * zIndex,
                            position.y + (tilePixelSize.GetHeight() / 10 + 3) -
                                (tilePixelSize.GetHeight() / 10 + 3) * zIndex,
                            tilePixelSize.GetWidth() * 2,
                            tilePixelSize.GetHeight() * 2, 10);

    dc.SetBrush(front);

    dc.DrawRoundedRectangle(
        position.x - (tilePixelSize.GetWidth() / 10 + 3) * zIndex,
        position.y - (tilePixelSize.GetHeight() / 10 + 3) * zIndex,
        tilePixelSize.GetWidth() * 2, tilePixelSize.GetHeight() * 2, 10);

    dc.SetBrush(_bgColor);

    if (tileImages[index].IsOk()) {
        wxPoint pos;
        pos.x = position.x + 10 - (tilePixelSize.GetWidth() / 10 + 3) * zIndex;
        pos.y = position.y + 10 - (tilePixelSize.GetHeight() / 10 + 3) * zIndex;

        if (tileImages[index].GetWidth() != tilePixelSize.x * 2)
            dc.DrawBitmap(tileImages[index].Scale(tilePixelSize.x * 2 - 20,
                                                  tilePixelSize.y * 2 - 20),
                          pos);
        else
            dc.DrawBitmap(tileImages[index], pos);
    }
}

void Drawer::resizeBg(const wxSize& resolution) {
    if (this->resolution != resolution) {
        this->resolution = resolution;
        composeBG();
    }
}

/**
 * Resizes tile and whole board bitmap size to the resolution, set in this
 * instance
 */
bool Drawer::resizeBoard(const TLVec& layout, const Dimensions& gridSize) {
    bool res = false;

    const int gridPoint = mmin(resolution.x / (gridSize.x * TILE_WIDTH),
                               resolution.y / (gridSize.y * TILE_HEIGHT));

    wxLogDebug(wxString::Format("Resize board: %i", gridPoint));

    if (gridPoint >= MIN_GRID_POINT) {
        if (gridPoint != prevGridPoint) {
            tablePixelRect.SetSize({gridPoint * TILE_WIDTH * gridSize.x,
                                    gridPoint * TILE_HEIGHT * gridSize.y});

            tilePixelSize.Set(gridPoint * TILE_WIDTH, gridPoint * TILE_HEIGHT);
        }

        tablePixelRect.SetPosition(
            {(resolution.x - tablePixelRect.width) / 2,
             (resolution.y - tablePixelRect.height) / 2});

        if (gridPoint != prevGridPoint) {
            composeBoard(layout, gridSize);
            res = true;
        }

        prevGridPoint = gridPoint;
    }

    return res;
}

wxPoint Drawer::toGrid(const wxPoint& point) const {
    wxPoint out(-1, -1);

    if (point.x >= tablePixelRect.x &&
        point.x <= tablePixelRect.x + tablePixelRect.width &&
        point.y >= tablePixelRect.y &&
        point.y <= tablePixelRect.y + tablePixelRect.height) {
        out.x = (point.x - tablePixelRect.x) / tilePixelSize.x;
        out.y = (point.y - tablePixelRect.y) / tilePixelSize.y;
    }

    return out;
}

wxPoint Drawer::fromGrid(int x, int y) const {
    return {x * tilePixelSize.x, y * tilePixelSize.y};
}

wxPoint Drawer::fromGrid(const wxPoint& point) const {
    return fromGrid(point.x, point.y);
}

wxSize Drawer::composeMinSize(const wxSize& gridSize) {
    wxSize ms;

    ms.SetWidth(MIN_GRID_POINT * TILE_WIDTH * gridSize.x);
    ms.SetHeight(MIN_GRID_POINT * TILE_HEIGHT * gridSize.y);

    wxLogDebug(wxString::Format("MinSize %i %i", ms.x, ms.y));

    return ms;
}