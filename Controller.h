#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "wxw.h"

#include "Drawer.h"
#include "XmlLayout.h"

class Controller {
public:
    Controller(Drawer& drawer);

    int stopwatch = 0;

    void resize(const wxSize& tableSize);

    void loadLayout(const wxString& path);

    wxPoint toGrid(const wxPoint& point);
    wxPoint fromGrid(const wxPoint& point);

    void select(TLSquare* card);

    TLVec* getTable();

    TLSquare* getCardByPosition(const wxPoint& point);
private:
    Drawer& drawer;
    XmlLayout layout;

    TLVec table;

    TLSquare* selected = nullptr;
};

#endif
