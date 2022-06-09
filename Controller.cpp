#include "Controller.h"

#include <exception>

static const std::array<uint8_t, 42> defaultCardsCounter{
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1};

void Controller::loadLayout(const wxString& path) {
    layout.openFile(path); // открываем файл карты

    gridSize = layout.getDimensions(); // получаем размеры карты

    table = TLVec( // создаём трёхмерный вектор из id карт
        gridSize.z,
        vector<vector<CardT>>(gridSize.x, vector<CardT>(gridSize.y, EMPTY)));

    layout.readLayout(table); // считываем формат поля из файла

    remaining = layout.getTilesNumber(); // получаем предполагаемое количество камней

    if (remaining == 144) // другие форматы не каноничны, поэтому мы их не поддерживаем
    // Заполняем массив-счётчик карт с различными id
        cardsCounter = defaultCardsCounter;
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
    srand(time(NULL)); // инициализируем генератор случайных чисел

    auto not_end = remaining; // сохраняем в отдельную переменную количество оставшихся камней

    PosSet positions; // инициализируем сет для хранения доступных для вставки позиций

    positions.insert(getRandLowest()); // вставляем случайную начальную позицию

    auto next_ptr = positions.begin(); // инициализируем указатель на позицию, куда будет вставляться следующий камень

    while (!positions.empty()) {
        int id = genRandId();

        emplace_table(id, *next_ptr, positions); // вставляем id в next_ptr
        not_end--; // уменьшаем счётчик оставшихся для вставки камней

        next_rand(positions, next_ptr, false, not_end); // Находим случайную новую позицию так, чтобы она не накрывала предыдущую

        if (id < 34) // если id парный
            cardsCounter[id]--; // уменьшаем счётчик карт этого id ещё на 1, так как вставим его ещё раз
        else
            id = getFreeSingularId(id);

        emplace_table(id, *next_ptr, positions);
        not_end--;

        next_rand(positions, next_ptr, true, not_end);
    }
}

wxPoint Controller::getRandLowest() const {
    int overall = gridSize.x * gridSize.y; // вычисляем количество позиций в горизонтальном "срезе" массива
    int x, y; // объявляем координаты для возвращаемой позиции

    do {
        int pos = rand() % overall; // получаем случайный номер позиции
        x = pos / gridSize.y; // вычисляем x
        y = pos % gridSize.y; // и y
    } while (table[0][x][y] != FREE); // повторяем цикл, если эта позиция недоступна для вставки

    return wxPoint(x, y); // возвращаем wxPoint
}

void Controller::emplace_table(CardT id, const ThreePoint& pos, PosSet& positions) {
    table[pos.z][pos.x][pos.y] = id;

    push_available(positions, pos); // записываем в сет новые позиции
}

#ifdef WXDEBUG // Если компилируем в режиме дебага

#include <wx/file.h>

void print_list(const PosSet& positions) {
    wxFile f("tmp.txt", wxFile::write_append); // Открываем файл для записи

    for (const auto& el : positions) // Итерируемся по всем позициям
        f.Write(itowxS(el.z) + " " + itowxS(el.x) + " " + itowxS(el.y) + "\n"); // Выводим координаты в файл

    f.Write("_ size: " + itowxS(positions.size()) + "\n"); // В конце выводим количество элементов 
}

#endif

void Controller::next_rand(PosSet& positions,
                              PosSet::iterator& ptr,
                              bool canOverlap, uint8_t& not_end) {
#ifdef WXDEBUG // Если компилируем в режиме дебага
    print_list(positions); // выводим список позиций
#endif

    ThreePoint prev = *ptr; // сохраняем предыдущее значение итератора

    positions.erase(ptr); // удаляем только что вставленный итератор

    if (not_end) { // если ещё есть камни для вставки
        if (positions.empty()) // если не осталось позиций,
            ptr = positions.insert(getRandLowest()).first; // вставляем любую позицию из нижней плоскости и используем её в следующий раз
        else { // иначе
            ptr = positions.begin(); // устанавливаем итератор в первую позицию

            int rand_d = rand() % positions.size(); // получаем случайное смещение внутри набора возможных позиций

            std::advance(ptr, rand_d); // смещаем итератор

            const auto rand_ptr = ptr; // сохраняем предыдущее положение итератора

            while (!canOverlap && ptr != positions.end() && wouldOverlap(prev, *ptr)) // Пока не найдём тот, что не будет закрывать только что вставленную позицию, если не canBeUp или дошли до конца набора
                ptr++; // наращиваем итератор

            if (ptr == positions.end()) { // если ни одна из позиций начиная с rand_ptr не подошла (нельзя выбирать накрывающую предыдущий камень и все позиции накрывают) 
                ptr = positions.begin(); // начинаем с начала

                while (!canOverlap && ptr != rand_ptr && wouldOverlap(prev, *ptr)) // Пока не найдём тот, что не будет закрывать только что вставленную позицию, если не canBeUp и не дошли до rand_ptr
                    ptr++; // наращиваем итератор
            }

            if (ptr == rand_ptr && !canOverlap && wouldOverlap(prev, *ptr)) { // если итератор совпадает с rand_ptr и при этом ptr перекрывает prev,
                if (not_end == positions.size()) // если уже все позиции добавлены в набор
                    ptr = positions.begin(); // просто выбираем первую из них
                else { // иначе
                    auto res = positions.insert(getRandLowest()); // пытаемся вставить вставляем случайную позицию в нижней плоскости 

                    while (!res.second) // пока не произошла вставка позиции в набор
                        res = positions.insert(getRandLowest()); // пытаемся вставить случайную позицию в нижней плоскости

                    ptr = res.first; // получаем итератор на только что вставленную позицию
                }
            }
        }
    }
}

bool Controller::wouldOverlap(const ThreePoint& prev, const ThreePoint& next) {
    table[next.z][next.x][next.y] = 1; // вставляем в позицию next временный камень

    bool res = !upFree(prev); // проверяем, будет ли свободен сверху камень в позиции prev

    table[next.z][next.x][next.y] = FREE; // удаляем временный камень

    return res; // возвращаем результат проверки
}

/**
 * Checks if position `p`, shifted by `d`, is not out of bounds of array `table` with dimensions `gridSize`
 */
bool Controller::corrInd(const ThreePoint& p, const ThreePoint& d) const {
    auto& gS = gridSize; // более короткий алиас для переменной

    return ((d.z == 0) || (d.z < 0 && p.z >= -d.z) || (d.z > 0 && p.z + d.z < gS.z)) &&
           ((d.x == 0) || (d.x < 0 && p.x >= -d.x) || (d.x > 0 && p.x + d.x < gS.x)) &&
           ((d.y == 0) || (d.y < 0 && p.y >= -d.y) || (d.y > 0 && p.y + d.y < gS.y));
}

/**
 * Checks if position `p`, shifted by `d`, is not out of bounds and available for insert (FREE)
 */
bool Controller::Free(const ThreePoint& p, const ThreePoint& d) const {
    return corrInd(p, d) && (table[p.z + d.z][p.x + d.x][p.y + d.y] == FREE);
}

/**
 * Checks if position `p`, shifted by `d`, is out of bounds or unavailable for insert (FREE)
 */
bool Controller::NFree(const ThreePoint& p, const ThreePoint& d) const {
    return !corrInd(p, d) || (table[p.z + d.z][p.x + d.x][p.y + d.y] != FREE);
}

/**
 * Pushes all positions, that are close to `pos`, available for insert and don't overlap other available positions
 */
void Controller::push_available(PosSet& positions,
                                 const ThreePoint& pos) const {
    auto& p = pos; // короткий алиас для переменной

    int z = pos.z, x = pos.x, y = pos.y; // "разбираем" объект pos на координаты

    // дальше идёт миллион условий, которые проще просто прочитать, нежели описывать, что они проверяют. В комментариях возле условий указано, в какую сторону относительно pos смещается вставляемая позиция

    if (NFree(p, {-1, -2, 0}) && NFree(p, {-1, -3, 0}) && NFree(p, {-1, -2, -1}) && NFree(p, {-1, -3, -1}) && NFree(p, {-1, -2, 1}) && NFree(p, {-1, -3, 1}) && Free(p, {0, -2, 0})) // left
        positions.emplace(z, x-2, y);
    if (NFree(p, {-1, 2, 0}) && NFree(p, {-1, 3, 0}) && NFree(p, {-1, 2, -1}) && NFree(p, {-1, 3, -1}) && NFree(p, {-1, 2, 1}) && NFree(p, {-1, 3, 1}) && Free(p, {0, 2, 0})) // right
        positions.emplace(z, x+2, y);

    if (NFree(p, {0, 0, -2})) { // half top
        if (NFree(p, {-1, -2, -1}) && NFree(p, {-1, -3, -1}) && NFree(p, {-1, -2, 0}) && NFree(p, {-1, -3, 0}) && NFree(p, {-1, -2, -2}) && NFree(p, {-1, -3, -2}) && NFree(p, {-1, -1, -2}) && Free(p, {0, -2, -1})) // left
            positions.emplace(z, x-2, y-1);
        if (NFree(p, {-1, 2, -1}) && NFree(p, {-1, 3, -1}) && NFree(p, {-1, 2, 0}) && NFree(p, {-1, 3, 0}) && NFree(p, {-1, 2, -2}) && NFree(p, {-1, 3, -2}) && NFree(p, {-1, 1, -2}) && Free(p, {0, 2, -1})) // right 
            positions.emplace(z, x+2, y-1);
    }

    if (NFree(p, {0, 0, 2})) { // half bottom 
        if (NFree(p, {-1, -2, 1}) && NFree(p, {-1, -3, 1}) && NFree(p, {-1, -2, 0}) && NFree(p, {-1, -3, 0}) && NFree(p, {-1, -2, 2}) && NFree(p, {-1, -3, 2}) && NFree(p, {-1, -1, 2}) && Free(p, {0, -2, 1})) // left
            positions.emplace(z, x-2, y+1);
        if (NFree(p, {-1, 2, 1}) && NFree(p, {-1, 3, 1}) && NFree(p, {-1, 2, 0}) && NFree(p, {-1, 3, 0}) && NFree(p, {-1, 2, 2}) && NFree(p, {-1, 3, 2}) && NFree(p, {-1, 1, 2}) && Free(p, {0, 2, 1})) // right
            positions.emplace(z, x+2, y+1);
    }

    if (NFree(p, {-1, 0, -2}) && NFree(p, {-1, 0, -3}) && NFree(p, {-1, -1, -2}) && NFree(p, {-1, -1, -3}) && NFree(p, {-1, 1, -2}) && NFree(p, {-1, 1, -3}) && Free(p, {0, 0, -2})) // top 
        positions.emplace(z, x, y-2);
    if (NFree(p, {-1, 0, 2}) && NFree(p, {-1, 0, 3}) && NFree(p, {-1, -1, 2}) && NFree(p, {-1, -1, 3}) && NFree(p, {-1, 1, 2}) && NFree(p, {-1, 1, 3}) && Free(p, {0, 0, 2})) // bottom 
        positions.emplace(z, x, y+2);

    /* Higher */

    if (Free(p, {1, 0, 0})) // straight 
        positions.emplace(z+1, x, y);

    if (NFree(p, {0, -1, -2}) && NFree(p, {0, 0, -2}) && NFree(p, {0, 1, -2}) && Free(p, {1, 0, -1})) // half top 
        positions.emplace(z+1, x, y-1);
    if (NFree(p, {0, -1, 2}) && NFree(p, {0, 0, 2}) && NFree(p, {0, 1, 2}) && Free(p, {1, 0, 1})) // half bottom
        positions.emplace(z+1, x, y+1);

    if (NFree(p, {0, -2, 0})) {// half left
        if (NFree(p, {0, -2, -1}) && NFree(p, {0, -2, 1}) && Free(p, {1, -1, 0})) // straight
            positions.emplace(z+1, x-1, y);
        
        if (NFree(p, {0, -2, -1}) && NFree(p, {0, -2, -2}) && NFree(p, {0, 0, -2}) && Free(p, {1, -1, -1})) // half top
            positions.emplace(z+1, x-1, y-1);

        if (NFree(p, {0, -2, 1}) && NFree(p, {0, -2, 2}) && NFree(p, {0, 0, 2}) && Free(p, {1, -1, 1})) // half bottom
            positions.emplace(z+1, x-1, y+1);
    }

    if (NFree(p, {0, 2, 0})) { // half right
        if (NFree(p, {0, 2, -1}) && NFree(p, {0, 2, 1}) && Free(p, {1, 1, 0})) // straight
            positions.emplace(z+1, x+1, y);

        if (NFree(p, {0, 2, -1}) && NFree(p, {0, 2, -2}) && NFree(p, {0, 0, -2}) && Free(p, {1, 1, -1})) // half top
            positions.emplace(z+1, x+1, y-1);

        if (NFree(p, {0, 2, 1}) && NFree(p, {0, 2, 2}) && NFree(p, {0, 0, 2}) && Free(p, {1, 1, 1})) // half bottom
            positions.emplace(z+1, x+1, y+1);
    }
}

/**
 * Removes all set stones and makes their positions free again
 */
void Controller::free_table() {
    // Итерируемся по массиву table размерности gridSize
    for (int z = 0; z < gridSize.z; z++)
        for (int x = 0; x < gridSize.x; x++)
            for (int y = 0; y < gridSize.y; y++) {
                CardT id = table[z][x][y]; // считываем id данной ячейки

                if (id >= 0) { // если это валидный id камня
                    cardsCounter[id]++; // наращиваем счётчик камней

                    table[z][x][y] = FREE;
                }
            }

    steps = decltype(steps)(); // сбрасываем стек steps (decltype удобен, ибо тогда не зависим от класса, просто вызываем стандартный конструктор)
}

void Controller::fillRandom() {
    srand(time(NULL)); // инициализируем генератор случайных чисел

    wxLogDebug(itowxS(remaining));

    auto not_end = remaining; // сохраняем количество оставшихся для вставки камней

    // итерируемся по всему массиву поля, пока не вставим все камни
    for (int z = 0; z < gridSize.z && not_end; z++)
        for (int x = 0; x < gridSize.x && not_end; x++)
            for (int y = 0; y < gridSize.y && not_end; y++)
                if (table[z][x][y] == FREE) { // если в эту позицию можно вставить камень
                    table[z][x][y] = genRandId(); // получаем случайный id и вставляем его туда
                    not_end--; // уменьшаем счётчик оставшихся камней
                }
}

CardT Controller::genRandId() {
    CardT id;

    do {
        id = rand() % TILE_IMAGES_N; // получаем случайное число-индекс в массиве имён камней
    } while (cardsCounter[id] == 0); // повторяем тело цикла, если эти id уже закончились

    cardsCounter[id]--; // уменьшаем счётчик оставшихся для вставки камней этого типа

    return id; // возвращаем полученный id
}

CardT Controller::getFreeSingularId(CardT prev) {
    CardT id = (prev < 38) ? 34 : 38; // устанавливаем первый из id, которые считаются одинаковыми с prev

    while (id < TILE_IMAGES_N && cardsCounter[id] == 0) // ищем в массиве оставшихся камней свободный id (если начинаем с 34, так как id выбираются парами, обязательно останется хотя бы один id, пренадлежащий этой группе (до 48), поэтому границу можно оставить одинаковой)
        id++;

    cardsCounter[id]--; // уменьшаем счётчик оставшихся id

    return id; // возвращаем его
}

/**
 * Gets pointer to top card by grid position 
 * It also changes point to top right coordinate of card
 */
CardT* Controller::getCardByPosition(ThreePoint& point) {
    int8_t topIndex = -1; // начинаем с -1, чтобы если не нашёлся ни один камень, получить невалидную позицию
    CardT* res = nullptr; // указатель на элемент массива

    ThreePoint realPos(point); // сохраняем копию позиции, чтобы при смещении не ломать позицию, для которой ищем

    // ищем верхнюю карту с верхним левым углом в данной позиции (нажатие в левую верхнюю четверть камня)
    for (int z = table.size() - 1; z >= 0; z--)
        if (table[z][point.x][point.y] >= 0) {
            if (z > topIndex) {
                topIndex = z;
                res = &table[z][point.x][point.y];
            }
            break;
        }

    // ищем верхнюю карту с верхним левым углом в данной позиции, смещённой на единицу влево (нажатие в правую верхнюю четверть камня)
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

    // ищем верхнюю карту с верхним левым углом в данной позиции, смещённой на единицу вверх (нажатие в левую нижнюю четверть камня)
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

    // ищем верхнюю карту с верхним левым углом в данной позиции, одновременно смещённой вверх и влево (нажатие в правую нижнюю четверть камня)
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
    
    // обновляем переданную позицию так, чтобы она указывала на левый верхний угол карты
    point.x = realPos.x;
    point.y = realPos.y;
    point.z = topIndex;

    return res;
}

bool Controller::available(const ThreePoint& point) const {
    return upFree(point) && sideFree(point);
}

bool Controller::upFree(const ThreePoint& point) const {

    if (point.z == table.size() - 1) // если находимся на самом верхнем уровне по оси z
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
    ThreePoint pos(drawer.toGrid(point)); // переводим позицию в координатах окна в координаты сетки

    if (pos.x > -1) { // если попали по полю
        CardT* card = getCardByPosition(pos); // получаем карту, в которую попали и смещаем позицию в её левый верхний угол

        if (pos.z >= 0 && available(pos)) { // если действительно получили карту и она доступна для убирания
            if (selected != nullptr && sameValues(*card, *selected) && // если уже есть выбранная карта и она такая же, как эта по значению,
                selected != card) { // но при этом не является тем же указателем
                steps.push({CardEntry{drawer.marked, *selected}, // сохраняем эту пару в истории
                            CardEntry{pos, *card}});

                *selected = MATCHED; // записываем в доску то, что эти карты убраны
                *card = MATCHED;

                selected = nullptr; // сбрасываем убранную карту

                remaining -= 2; // уменьшаем счётчик оставшихся для убирания карт на 1

                drawer.marked = {-1, -1, -1}; // сбрасываем координаты выбранной карты для "художника"
            } else {
                selected = card; // устанавливаем указатель на выбранную сейчас карту
                drawer.marked = pos; // устанавливаем координаты выбранной карты для "художника"
            }
        }
    }
}

bool Controller::sameValues(CardT a, CardT b) const {
    if (a == b) // если id карт равны
        return true;
    else if (a >= 38 && b >= 38) // или они входят в одну 
        return true;
    else if (a >= 34 && a <= 37 && b >= 34 && b <= 37) // из групп, где каждой карты по одной,но при этом все они считаются одинаковыми
        return true;

    return false;
}

void Controller::undo() {
    if (steps.size()) { // если есть шаги для отмены
        for (const CardEntry& entry : steps.top()) // в цикле по каждому из пары камней
            table[entry.pos.z][entry.pos.x][entry.pos.y] = entry.id; // возвращаем его на доску

        remaining += 2; // наращиваем счётчик оставшихся для уборки камней 
        steps.pop(); // удаляем только что восстановленные камни из истории
    }
}

bool Controller::gameStarted() const {
    return stopwatch > 0; // если счётчик таймера наращивается, игра началась
}