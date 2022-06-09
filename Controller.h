#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <array>
#include <stack>

#include "wxw.h"

#include "Drawer.h"
#include "XmlLayout.h"

class Controller {
public:
    Controller(Drawer& drawer) : drawer(drawer){};

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

    wxPoint getRandLowest() const;

    void emplace_table(CardT id, const ThreePoint& pos, PosSet& positions);
    void next_rand(PosSet& positions,
                      PosSet::iterator& ptr, bool canOverlap, uint8_t& not_end);

    bool wouldOverlap(const ThreePoint& prev, const ThreePoint& next);

    bool corrInd(const ThreePoint& p, const ThreePoint& d) const;
    bool Free(const ThreePoint& p, const ThreePoint& d) const;
    bool NFree(const ThreePoint& p, const ThreePoint& d) const;

    void push_available(PosSet& positions, const ThreePoint& pos) const;

    void fillRandom();

    CardT getFreeSingularId(CardT prev);
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
