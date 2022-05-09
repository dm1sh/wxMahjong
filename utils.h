#ifndef UTILS_H
#define UTILS_H

#include "wxw.h"

#include <vector>

using std::vector;

wxString LTimeToStr(int time);
int upDiv(int a, int b);
wxString itowxS(int a);

#define min(a, b) (a + b - abs(a - b)) / 2
#define max(a, b) (a + b + abs(a - b)) / 2

using CardT = int16_t;

class Dimensions : public wxSize {
public:
    Dimensions(int _z, int _x, int _y): wxSize(_x, _y), z(_z) {};
    Dimensions(): wxSize(), z(0) {};
    int z;
};

class ThreePoint {
public:
    ThreePoint(int _z, int _x, int _y): x(_x), y(_y), z(_z) {};
    ThreePoint(): x(0), y(0), z(0) {};
    int x;
    int y;
    int z;
};

using TLVec = vector<vector<vector<CardT>>>;

#endif
