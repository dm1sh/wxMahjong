#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <array>
#include <set>
#include <stack>

#include "wxw.h"

#include "Drawer.h"
#include "XmlLayout.h"

class Controller {
public:
    Controller(Drawer& drawer);

    int stopwatch = -1;

    void loadLayout(const wxString& path);

    void handleClick(const wxPoint& point);

    TLVec& getTable();

    void free_table();

    void fill(bool solveable);

    uint8_t remaining;

    void undo();

    bool gameStarted() const;

    Dimensions gridSize;

private:
    Drawer& drawer;
    XmlLayout layout;

    TLVec table;

    CardT* selected = nullptr;

    void fillSolveableTable();
    wxPoint getRandLowest();
    void emplace_rand(int id, std::set<ThreePoint>& positions,
                      std::set<ThreePoint>::iterator& next_ptr, bool canBeUp);
    bool wouldBeUpFree(const ThreePoint& prev, const ThreePoint& next);
    void push_available(std::set<ThreePoint>& positions, const ThreePoint& pos);

    void fillRandom();

    CardT genRandId();

    CardT* getCardByPosition(ThreePoint& point);

    bool available(const ThreePoint& point) const;
    bool upFree(const ThreePoint& point) const;
    bool sideFree(const ThreePoint& point) const;

    bool sameValues(CardT a, CardT b) const;

    std::array<uint8_t, TILE_IMAGES_N> cardsCounter;

    std::stack<std::array<CardEntry, 2>> steps;
};

#endif
