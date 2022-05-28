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
    time_t start_time = time(NULL);

    std::list<ThreePoint> positions;

    int overall = gridSize.z * gridSize.x * gridSize.y;
    int pos = rand() % overall;
    int z, x, y;

    do {
        z = pos / (gridSize.x * gridSize.y);
        x = (pos / gridSize.y) % gridSize.x;
        y = pos % gridSize.y;
    } while (table[z][x][y] != FREE);

    positions.push_back({z, x, y});

    int past_pos = 0;
    auto past_ptr = positions.begin();

    while (!positions.empty()) {
        int id = genRandId();

        if (id < 34) {
            past_pos = emplace_rand(id, positions, past_pos, past_ptr);
            past_pos = emplace_rand(id, positions, past_pos, past_ptr);

            cardsCounter[id]--;
        } else
            emplace_rand(id, positions, past_pos, past_ptr);
    }

    wxLogInfo(
        wxString::Format("Filling took %i seconds", start_time - time(NULL)));
}

int Controller::emplace_rand(int id, std::list<ThreePoint> positions,
                             int past_pos,
                             std::list<ThreePoint>::iterator past_ptr) {
    int d = rand() % positions.size() - past_pos;

    if (d > 0)
        for (int i = 0; i < d; i++)
            past_ptr++;
    else
        for (int i = 0; i > d; i--)
            past_ptr--;

    past_pos += d;

    table[past_ptr->z][past_ptr->x][past_ptr->y] = id;

    // if ()
    // positions.push_back({})

    /**
     * TODO: random move and positions adding
     */

    return 0;
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

    wxLogDebug(wxString::Format("%i", remaining));

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
        auto card = getCardByPosition(pos);

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