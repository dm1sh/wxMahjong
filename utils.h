#ifndef UTILS_H
#define UTILS_H

#include "wxw.h"

#include <set>
#include <vector>
#include <iterator>

using std::vector;

wxString LTimeToStr(int time);
int upDiv(int a, int b);
wxString itowxS(int a);
wxString PRemaining(uint8_t remaining);

#define mmin(a, b) (a + b - abs(a - b)) / 2
#define mmax(a, b) (a + b + abs(a - b)) / 2

using CardT = int16_t;

class Dimensions : public wxSize {
public:
    Dimensions(int _z, int _x, int _y) : wxSize(_x, _y), z(_z){};
    Dimensions() : wxSize(), z(0){};
    int z;
};

class ThreePoint {
public:
    constexpr ThreePoint(int _z, int _x, int _y) : x(_x), y(_y), z(_z){};
    ThreePoint(const wxPoint& a) : x(a.x), y(a.y), z(0){};
    ThreePoint() : x(0), y(0), z(0){};

    bool operator<(const ThreePoint& b) const {
        return z * 144 * 144 + x * 144 + y < b.z * 144 * 144 + b.x * 144 + b.y;
    }

    bool operator==(const ThreePoint& b) {
        return z == b.z && x == b.x && y == b.y;
    }

    int x;
    int y;
    int z;
};

class CardEntry {
public:
    ThreePoint pos;
    CardT id;
};

using TLVec = vector<vector<vector<CardT>>>;

enum Values { MATCHED = -3, EMPTY, FREE };

bool isPositive(const wxSize& size);

void cyclic_shift(std::set<ThreePoint>::iterator& it, const std::set<ThreePoint>& cont);

#endif
