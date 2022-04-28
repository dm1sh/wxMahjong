#include "Controller.h"
#include "utils.h"
#include <wx/xml/xml.h>

Controller::Controller(Drawer& drawer): drawer(drawer) {};

void Controller::resize(const wxSize& tableSize) {
    wxSize& resolution = drawer.resolution;
    wxSize& gridSize = drawer.gridSize;
    wxRect& tablePixelRect = drawer.tablePixelRect;

    resolution = tableSize;

    int gridPoint = min(resolution.x / (gridSize.x * TILE_WIDTH),
                        resolution.y / (gridSize.y * TILE_HEIGHT));
    
    if (gridPoint > 2) {
        tablePixelRect.SetSize({gridPoint * TILE_WIDTH * gridSize.x, gridPoint * TILE_HEIGHT * gridSize.y});

        drawer.tilePixelSize.Set(gridPoint * TILE_WIDTH, gridPoint * TILE_HEIGHT);
    }

    tablePixelRect.SetPosition({(resolution.x - tablePixelRect.width) / 2,
                                (resolution.y - tablePixelRect.height) / 2});

    drawer.setBG(tableSize);
    drawer.initScreen(tableSize, table);
}

wxPoint Controller::toGrid(const wxPoint& point) {
    wxPoint out(-1, -1);

    wxRect& tablePixelRect = drawer.tablePixelRect;

    if (point.x >= tablePixelRect.x &&
        point.x <= tablePixelRect.x + tablePixelRect.width &&
        point.y >= tablePixelRect.y &&
        point.y <= tablePixelRect.y + tablePixelRect.height)
    {
        out.x = (point.x - tablePixelRect.x) / drawer.tilePixelSize.x;
        out.y = (point.y - tablePixelRect.y) / drawer.tilePixelSize.y;
    }

    return out;
}

wxPoint Controller::fromGrid(const wxPoint& point) {
    return { drawer.tablePixelRect.x + point.x * drawer.tilePixelSize.x,
             drawer.tablePixelRect.y + point.y * drawer.tilePixelSize.y };
}

void Controller::loadLayout(const wxString& path) {
    layout.openFile(path);

    drawer.gridSize = layout.getDimensions();

    table = TLVec(drawer.gridSize.x, wxVector<wxVector<TLSquare>>(drawer.gridSize.y, wxVector<TLSquare>()));

    layout.readLayout(table);
}

TLVec* Controller::getTable() {
    return &table;
}

TLSquare* Controller::getCardByPosition(const wxPoint& point) {
    int8_t topIndex = -1;
    auto res = table[0][0].rend();

    for (auto el = table[point.x][point.y].rbegin(); el != table[point.x][point.y].rend(); ++el)
        if (el->second != (uint8_t)-1) {
            auto d = el->first;
            if (d > topIndex) {
                topIndex = d;
                res = el;
            }
            break;
        }

    if (point.x > 0)
        for (auto el = table[point.x-1][point.y].rbegin(); el != table[point.x-1][point.y].rend(); ++el)
            if (el->second != (uint8_t)-1) {
                auto d = el->first;
                if (d > topIndex) {
                    topIndex = d;
                    res = el;
                }
                break;
            }

    if (point.y > 0)
        for (auto el = table[point.x][point.y-1].rbegin(); el != table[point.x][point.y-1].rend(); ++el)
            if (el->second != (uint8_t)-1) {
                auto d = el->first;
                if (d > topIndex) {
                    topIndex = d;
                    res = el;
                }
                break;
            }

    if (point.x > 0 && point.y > 0)
        for (auto el = table[point.x-1][point.y-1].rbegin(); el != table[point.x-1][point.y-1].rend(); ++el)
            if (el->second != (uint8_t)-1) {
                auto d = el->first;
                if (d > topIndex) {
                    topIndex = d;
                    res = el;
                }
                break;
            }

    return &(*res);
}

void Controller::select(TLSquare* card) {
    if (selected != nullptr && selected->second == card->second && selected != card) {
        selected->second = -1;
        card->second = -1;
        selected = nullptr;
        
        drawer.initScreen(drawer.tableSize, table);
    } else 
        selected = card;
}
