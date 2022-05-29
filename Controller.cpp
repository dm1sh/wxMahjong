#include "Controller.h"

Controller::Controller(Drawer& drawer) : drawer(drawer){};

void Controller::loadLayout(const wxString& path) {
    layout.openFile(path);

    gridSize = layout.getDimensions();

    table = TLVec(
        gridSize.z,
        vector<vector<CardT>>(gridSize.x, vector<CardT>(gridSize.y, EMPTY)));

    layout.readLayout(table);

    remaining = layout.getTilesNumber();

    if (remaining == 144) {
        for (int i = 0; i < 34; i++)
            cardsCounter[i] = 4;
        for (int i = 34; i < TILE_IMAGES_N; i++)
            cardsCounter[i] = 1;
    }
}

TLVec& Controller::getTable() {
    return table;
}

void Controller::fill(bool solveable) {
    if (solveable)
        fillSolveableTable();
    else
        fillRandom();
}

void Controller::fillSolveableTable() {
    srand(time(NULL));

    auto not_end = remaining;

    std::set<ThreePoint> positions;

    positions.insert(getRandLowest());

    auto next_ptr = positions.begin();

    while (!positions.empty() || (not_end && !(positions.insert(getRandLowest()), positions.empty()))) {
        int id = genRandId();

        if (id < 34) {
            emplace_rand(id, positions, next_ptr, true);
            emplace_rand(id, positions, next_ptr, false);

            cardsCounter[id]--;
            not_end -= 2;
        } else {
            emplace_rand(id, positions, next_ptr, true);
            not_end--;
        }
    }
}

wxPoint Controller::getRandLowest() {
    int overall = gridSize.x * gridSize.y;
    int x, y;

    do {
        int pos = rand() % overall;
        x = (pos / gridSize.y) % gridSize.x;
        y = pos % gridSize.y;
    } while (table[0][x][y] != FREE);

    return {x, y};
}

#include <wx/file.h>

void print_list(const std::set<ThreePoint>& positions) {
    wxFile f("tmp.txt", wxFile::write_append);
    
    for (const auto& el : positions)
        f.Write(itowxS(el.z) + " " + itowxS(el.x) + " " + itowxS(el.y) + "\n");
    
    f.Write("_ size: " + itowxS(positions.size()) + "\n");
}

void Controller::emplace_rand(int id, std::set<ThreePoint>& positions,
                              std::set<ThreePoint>::iterator& next_ptr,
                              bool canBeUp) {
    print_list(positions);

    table[next_ptr->z][next_ptr->x][next_ptr->y] = id;

    push_available(positions, *next_ptr);

    auto prev_ptr = next_ptr;
    auto prev = *next_ptr;

    do {
        next_ptr++;
        
        if (next_ptr == positions.end())
            next_ptr = positions.begin();
    } while (!canBeUp && !wouldBeUpFree(prev, *next_ptr));

    positions.erase(prev_ptr);
}

bool Controller::wouldBeUpFree(const ThreePoint& prev, const ThreePoint& next) {
    table[next.z][next.x][next.y] = 1;

    bool res = upFree(prev);

    table[next.z][next.x][next.y] = FREE;

    return res;
}

void Controller::push_available(std::set<ThreePoint>& positions,
                                const ThreePoint& pos) {
    int z = pos.z, x = pos.x, y = pos.y;

    if (x >= 2 && table[z][x-2][y] == FREE) // left
        positions.emplace(z, x-2, y);
    if (x + 2 < gridSize.x && table[z][x+2][y] == FREE) // right
        positions.emplace(z, x+2, y);

    if (y >= 1 && (y < 2 || table[z][x][y-2] != FREE)) { // half bottom
        if (x >= 2 && table[z][x-2][y-1] == FREE) // left
            positions.emplace(z, x-2, y-1);
        if (x + 2 < gridSize.x && table[z][x+2][y-1] == FREE) // right
            positions.emplace(z, x+2, y-1);
    }

    if (y + 1 < gridSize.y && (y + 2 >= gridSize.y || table[z][x][y+2] != FREE)) { // half bottom
        if (x >= 2 && table[z][x-2][y+1] == FREE) // left
            positions.emplace(z, x-2, y+1);
        if (x + 2 < gridSize.x && table[z][x+2][y+1] == FREE) // right
            positions.emplace(z, x+2, y+1);
    }

    if (y >= 2 && table[z][x][y-2] == FREE) // up
        positions.emplace(z, x, y-2);
    if (y + 2 < gridSize.y && table[z][x][y+2] == FREE) // bottom
        positions.emplace(z, x, y+2);

    if (z + 1 < gridSize.z) { // higher
        if (table[z+1][x][y] == FREE) // straight
            positions.emplace(z+1, x, y);

        if (y >= 1 && (y < 2 || table[z][x][y-2] != FREE) && table[z+1][x][y-1] == FREE) // half top
            positions.emplace(z+1, x, y-1);
        if (y + 1 < gridSize.y && (y + 2 >= gridSize.y || table[z][x][y+2] != FREE) && table[z+1][x][y+1] == FREE) // half bottom
            positions.emplace(z+1, x, y+1);

        if (x >= 1 && (x < 2 || table[z][x-2][y] != FREE)) {// half left
            if (table[z+1][x-1][y] == FREE) // straight
                positions.emplace(z+1, x-1, y);
            
            if (y >= 1 && (x < 2 || table[z][x-2][y-1] != FREE) && table[z+1][x-1][y-1] == FREE) // half top
                positions.emplace(z+1, x-1, y-1);

            if (y + 1 < gridSize.y && (x < 2 || table[z][x-2][y+1] != FREE) && table[z+1][x-1][y+1] == FREE) //half bottom
                positions.emplace(z+1, x-1, y+1);
        }

        if (x + 1 < gridSize.x && (x + 2 >= gridSize.x || table[z][x+2][y] != FREE)) { // half right
            if (table[z+1][x+1][y] == FREE) // straight
                positions.emplace(z+1, x+1, y);

            if (y >= 1 && (x + 2 >= gridSize.x || table[z][x+2][y-1] != FREE) && table[z+1][x+1][y-1] == FREE) // half top
                positions.emplace(z+1, x+1, y-1);

            if (y + 1 < gridSize.y && (x + 2 >= gridSize.x || table[z][x+2][y+1] != FREE) && table[z+1][x+1][y+1] == FREE) //half bottom
                positions.emplace(z+1, x+1, y+1);
        }
    }
}

void Controller::free_table() {
    for (int z = 0; z < gridSize.z; z++)
        for (int x = 0; x < gridSize.x; x++)
            for (int y = 0; y < gridSize.y; y++) {
                CardT id = table[z][x][y];

                if (id >= 0) {
                    cardsCounter[id]++;

                    table[z][x][y] = FREE;
                }
            }

    steps = decltype(steps)();
}

void Controller::fillRandom() {
    srand(time(NULL));

    wxLogDebug(itowxS(remaining));

    auto not_end = remaining;

    for (int z = 0; z < gridSize.z && not_end; z++)
        for (int x = 0; x < gridSize.x && not_end; x++)
            for (int y = 0; y < gridSize.y && not_end; y++)
                if (table[z][x][y] == FREE) {
                    table[z][x][y] = genRandId();
                    not_end--;
                }
}

CardT Controller::genRandId() {
    CardT id;

    int w = 0;

    do {
        id = rand() % TILE_IMAGES_N;
        w++;
    } while (cardsCounter[id] == 0);

    cardsCounter[id]--;

    return id;
}

/**
 * It also changes point to top right coordinate of card
 */
CardT* Controller::getCardByPosition(ThreePoint& point) {
    int8_t topIndex = -1;
    CardT* res = nullptr;

    ThreePoint realPos = point;

    for (int z = table.size() - 1; z >= 0; z--)
        if (table[z][point.x][point.y] >= 0) {
            if (z > topIndex) {
                topIndex = z;
                res = &table[z][point.x][point.y];
            }
            break;
        }

    if (point.x > 0)
        for (int z = table.size() - 1; z >= 0; z--)
            if (table[z][point.x - 1][point.y] >= 0) {
                if (z > topIndex) {
                    topIndex = z;
                    res = &table[z][point.x - 1][point.y];

                    realPos.x = point.x - 1;
                    realPos.y = point.y;
                }
                break;
            }

    if (point.y > 0)
        for (int z = table.size() - 1; z >= 0; z--)
            if (table[z][point.x][point.y - 1] >= 0) {
                if (z > topIndex) {
                    topIndex = z;
                    res = &table[z][point.x][point.y - 1];

                    realPos.x = point.x;
                    realPos.y = point.y - 1;
                }
                break;
            }

    if (point.x > 0 && point.y > 0)
        for (int z = table.size() - 1; z >= 0; z--)
            if (table[z][point.x - 1][point.y - 1] >= 0) {
                if (z > topIndex) {
                    topIndex = z;
                    res = &table[z][point.x - 1][point.y - 1];

                    realPos.x = point.x - 1;
                    realPos.y = point.y - 1;
                }
                break;
            }

    point.x = realPos.x;
    point.y = realPos.y;
    point.z = topIndex;

    return res;
}

bool Controller::available(const ThreePoint& point) const {
    return upFree(point) && sideFree(point);
}

bool Controller::upFree(const ThreePoint& point) const {

    if (point.z == table.size() - 1)
        return true;

    return !((table[point.z + 1][point.x][point.y] >= 0) ||
             (point.x > 0 && table[point.z + 1][point.x - 1][point.y] >= 0) ||
             (point.y > 0 && table[point.z + 1][point.x][point.y - 1] >= 0) ||
             (point.x > 0 && point.y > 0 &&
              table[point.z + 1][point.x - 1][point.y - 1] >= 0) ||
             (point.x < table[point.z].size() - 1 &&
              table[point.z + 1][point.x + 1][point.y] >= 0) ||
             (point.y < table[point.z][point.x].size() - 1 &&
              table[point.z + 1][point.x][point.y + 1] >= 0) ||
             (point.x < table[point.z].size() - 1 &&
              point.y < table[point.z][point.x].size() - 1 &&
              table[point.z + 1][point.x + 1][point.y + 1] >= 0) ||
             (point.x > 0 && point.y < table[point.z][point.x].size() - 1 &&
              table[point.z + 1][point.x - 1][point.y + 1] >= 0) ||
             (point.x < table[point.z].size() - 1 && point.y > 0 &&
              table[point.z + 1][point.x + 1][point.y - 1] >= 0));
}

bool Controller::sideFree(const ThreePoint& point) const {
    bool lfree = true;
    bool rfree = true;

    if (point.x > 1)
        lfree =
            !((point.y > 0 && table[point.z][point.x - 2][point.y - 1] >= 0) ||
              (table[point.z][point.x - 2][point.y] >= 0) ||
              (point.y < table[point.z][point.x].size() - 1 &&
               table[point.z][point.x - 2][point.y + 1] >= 0));

    if (point.x < table[point.z].size() - 2)
        rfree =
            !((point.y > 0 && table[point.z][point.x + 2][point.y - 1] >= 0) ||
              (table[point.z][point.x + 2][point.y] >= 0) ||
              (point.y < table[point.z][point.x].size() - 1 &&
               table[point.z][point.x + 2][point.y + 1] >= 0));

    return lfree || rfree;
}

void Controller::handleClick(const wxPoint& point) {
    wxPoint posPlain = drawer.toGrid(point);

    ThreePoint pos = {-1, posPlain.x, posPlain.y};

    if (pos.x > -1) {
        CardT* card = getCardByPosition(pos);

        if (pos.z >= 0 && available(pos)) {
            if (selected != nullptr && sameValues(*card, *selected) &&
                selected != card) {
                steps.push({CardEntry{drawer.marked, *selected},
                            CardEntry{pos, *card}});

                *selected = MATCHED;
                *card = MATCHED;

                selected = nullptr;

                remaining -= 2;

                drawer.marked = {-1, -1, -1};
            } else {
                selected = card;
                drawer.marked = pos;
            }
        }
    }
}

bool Controller::sameValues(CardT a, CardT b) const {
    if (a == b)
        return true;
    else if (a >= 38 && b >= 38)
        return true;
    else if (a >= 34 && a <= 37 && b >= 34 && b <= 37)
        return true;

    return false;
}

void Controller::undo() {
    if (steps.size()) {
        for (const CardEntry& entry : steps.top()) {
            table[entry.pos.z][entry.pos.x][entry.pos.y] = entry.id;
            cardsCounter[entry.id]++;
        }

        remaining += 2;
        steps.pop();
    }
}

bool Controller::gameStarted() const {
    return stopwatch > 0;
}