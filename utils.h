#ifndef UTILS_H
#define UTILS_H

#include "wxw.h"

wxString LTimeToStr(int time);
int upDiv(int a, int b);
wxString itowxS(int a);

#define min(a, b) (a + b - abs(a - b)) / 2
#define max(a, b) (a + b + abs(a - b)) / 2

using TLSquare = std::pair<uint8_t, uint8_t>;
using TLVec = wxVector<wxVector<wxVector<TLSquare>>>;

#endif
