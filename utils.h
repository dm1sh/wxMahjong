#ifndef UTILS_H
#define UTILS_H

#include "wxw.h"

#include <unordered_set>
#include <vector>
#include <iterator>

using std::vector;

wxString LTimeToStr(int time);
wxString itowxS(int a);
wxString PRemaining(uint8_t remaining);

#define mmin(a, b) (a + b - abs(a - b)) / 2 // среднее арифметическое минус половина разницы
#define mmax(a, b) (a + b + abs(a - b)) / 2 // среднее арифметическое плюс половина разницы

using CardT = int16_t;

struct Dimensions : wxSize { // используется там, где необходимо задать размеры в трёх координатах
    Dimensions(int _z, int _x, int _y) : z(_z), wxSize(_x, _y){};
    Dimensions() : wxSize(), z(0){};

    int z;
};

struct ThreePoint : wxPoint { // используется там, где необходимо задать положение в трёх координатах. Так же засчёт наследования от wxPoint, может быть передан в функцию, принимающую wxPoint
    ThreePoint(int _z, int _x, int _y) : z(_z), wxPoint(_x, _y){};
    ThreePoint(const wxPoint& a) : z(0), wxPoint(a.x, a.y){};
    ThreePoint() : z(0), wxPoint(0, 0){};

    bool operator==(const ThreePoint& b) const { // требуется для того, чтобы использовать в std::unordered_set (так как это множетсво)
        return z == b.z && x == b.x && y == b.y;
    }

    int z;
};

// доопределяем функтор std::hash для параметра шаблона ThreePoint, чтобы использовать в std::unordered_set
namespace std {
    template<> struct hash<ThreePoint> {
        size_t operator()(const ThreePoint& p) const {
            return std::hash<uint32_t>()(p.z * 288 * 288 + p.x * 288 + p.y); // координаты точки можно представить как число в 288-ичной системе счисления, так как в крайнем случае (если индексируется прямая линия из всех камней), максимальная координата будет 144*2-1. хэш для этого числа может иметь максимальное значение 287*288*288+287*288+287, что больше, чем 2^16, но меньше, чем 2^32, поэтому используем uint32_t (на самом деле это преобразуется в простое статическое преобразование типа к size_t, но документация совертует делать так)
        }
    };
}

using PosSet = std::unordered_set<ThreePoint>; // алиас для типа

struct CardEntry { // используется как элемент стека, хранящего историю ходов
    ThreePoint pos;
    CardT id;
};

using TLVec = vector<vector<vector<CardT>>>;

enum Values { // перечисление псевдо-id в таблице для не занятых реальными id камней позиций
     MATCHED = -3, // уже убран
     EMPTY, // не должно быть камня
     FREE // доступен для вставки камня
}; 

bool isPositive(const wxSize& size);

#endif
