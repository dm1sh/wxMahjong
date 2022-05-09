#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <array>
#include <list>

#include "wxw.h"

#include "Drawer.h"
#include "XmlLayout.h"

class Controller {
public:
    Controller(Drawer& drawer);

    int stopwatch = -1;

    void resize(const wxSize& tableSize);

    void loadLayout(const wxString& path);

    bool available(const ThreePoint& point);
    bool upFree(const ThreePoint& point);
    bool sideFree(const ThreePoint& point);

    void select(CardT* card);

    TLVec& getTable();

    void fillSolveableTable();
    void fillRandom();

    CardT* getCardByPosition(ThreePoint& point);

    std::array<uint8_t, TILE_IMAGES_N>cardsCounter;

    uint8_t remaining;
private:
    Drawer& drawer;
    XmlLayout layout;

    TLVec table;

    CardT* selected = nullptr;

    int emplace_rand(int id, std::list<ThreePoint> positions, int past_pos, std::list<ThreePoint>::iterator past_ptr);

    int genRandId();

    bool sameValues(CardT a, CardT b);
};

#endif
