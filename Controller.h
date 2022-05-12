#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <array>
#include <list>
#include <stack>

#include "wxw.h"

#include "Drawer.h"
#include "XmlLayout.h"

class Controller {
public:
    Controller(Drawer& drawer);

    int stopwatch = -1;

    void resize(const wxSize& tableSize);

    void loadLayout(const wxString& path);

    void handleClick(const wxPoint& point);

    TLVec& getTable();

    void free_table();

    void fill(bool solveable);

    uint8_t remaining;

    void undo();
private:
    Drawer& drawer;
    XmlLayout layout;

    TLVec table;

    CardT* selected = nullptr;

    void fillSolveableTable();
    void fillRandom();

    int emplace_rand(int id, std::list<ThreePoint> positions, int past_pos, std::list<ThreePoint>::iterator past_ptr);

    CardT genRandId();

    CardT* getCardByPosition(ThreePoint& point);

    bool available(const ThreePoint& point);
    bool upFree(const ThreePoint& point);
    bool sideFree(const ThreePoint& point);

    bool sameValues(CardT a, CardT b);

    std::array<uint8_t, TILE_IMAGES_N>cardsCounter;

    std::stack<std::array<CardEntry, 2>> steps;
};

#endif
