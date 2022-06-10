#include "Drawer.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>

const char* tileImageNames[TILE_IMAGES_N] = {
    // clang-format off
    "Pin1",    "Pin2",    "Pin3",    "Pin4",    "Pin5",    "Pin6",   "Pin7",    "Pin8",    "Pin9",
    "Sou1",    "Sou2",    "Sou3",    "Sou4",    "Sou5",    "Sou6",   "Sou7",    "Sou8",    "Sou9",
    "Man1",    "Man2",    "Man3",    "Man4",    "Man5",    "Man6",   "Man7",    "Man8",    "Man9",
    "Chun",    "Haku",    "Hatsu",
    "Nan",     "Pei",     "Shaa",    "Ton",
    "Flower1", "Flower2", "Flower3", "Flower4",
    "Season1", "Season2", "Season3", "Season4"
    // clang-format on
};

Drawer::Drawer() : marked{-1, -1, -1} {
    wxString path = wxStandardPaths::Get().GetUserDataDir() + wxFileName::GetPathSeparator() + _("tiles") + wxFileName::GetPathSeparator();

    for (int i = 0; i < TILE_IMAGES_N; i++) { // В цикле по именам изображений
        bool succeed = tileImages[i].LoadFile( // загружаем их
            path + _(tileImageNames[i]) + _(".png"), // из стандартной папки для ресурсов приложения
            wxBITMAP_TYPE_PNG); // в формате png
        if (!succeed) // В случае ошибки заргрузки выводим сообщение об этом
            wxLogDebug(_("failed to load tile ./resources/tiles/") + // с путём,
                       _(tileImageNames[i]) + _(".png with index") + // именем файла
                       wxString::Format("%i", i)); // и индексом в массиве
    }
}

void Drawer::drawTable(wxDC& dc) const {
    dc.DrawBitmap(bgBitmap, 0, 0, false); // отрисовываем в dc битмап с фоном, начиная из левого верхнего угла без маски
    if (boardBitmap.IsOk()) { // Если изображение доски построено
        wxLogDebug("Drawing board");
        dc.DrawBitmap(boardBitmap, boardPixelRect.GetPosition(), true); // отрисовываем битмап с ней в установленном при рейсайзе положении, используя маску
    }
}

const wxColor lGreen{0x07, 0x55, 0x2b};
const wxColor dGreen{0x01, 0x2d, 0x16};

void Drawer::composeBG() {
    bgBitmap = wxBitmap(resolution); // создаём битмап размером со всю панейль

    wxLogDebug(
        wxString::Format("Rebuild bg %i %i", resolution.x, resolution.y));

    wxMemoryDC dc; // создаём dc в памяти
    dc.SelectObject(bgBitmap); // выбираем свежесозданный битмап как холст для рисования

    dc.GradientFillConcentric(wxRect(wxPoint(0, 0), resolution), lGreen, // рисуем радиальный градиент на весь битмап, переходящий от светлозелёного в центре 
                              dGreen); // к тёмнозелёному по краям
}

void Drawer::composeBoard(const TLVec& table, const Dimensions& gridSize) {
    boardBitmap = wxBitmap(boardPixelRect.GetSize()); // Создаём битмап согласно размерам, полученным при последнем ресайзе окна

    wxLogDebug(_("Rebuild board"));

    wxMemoryDC dc; // создаём dc в памяти 
    dc.SelectObject(boardBitmap); // выбираем свежесозданный битмап как холст для рисования

    // итерируемся по всем индексам массива table
    for (int z = 0; z < gridSize.z; z++)
        for (int x = 0; x < gridSize.x; x++)
            for (int y = 0; y < gridSize.y; y++) {
                CardT c = table[z][x][y]; // получаем id в данной позиции
                if (c >= 0) // если тут стоит камень
                    drawTile(dc, c, fromGrid(x, y), z); // отрисовываем его
            }

    dc.SelectObject(wxNullBitmap); // отцепляем dc (обычно это происходит при выходе объекта за поле видимости, но здесь это нужно сделать вручную, чтобы создать маску)

    wxMask* mask = new wxMask(boardBitmap, wxColor(0x00, 0x00, 0x00)); // создаём маску по полю, чтобы при отрисовке там, где нет камней не было чёрного фона
    boardBitmap.SetMask(mask); // устанавливаем маску для битмапа
}

void Drawer::drawTile(wxDC& dc, int8_t index, const wxPoint& position,
                      uint8_t zIndex) const {
    wxBrush _bgColor = dc.GetBrush(); // сохраняем цвет кисти, которая была в dc по умолчанию

    wxBrush front = wxColor(0xff, 0xff, 0xff); // создаём кисти для лицевой части камня
    wxBrush back = wxColor(0xc8, 0xc8, 0xc8); // и подложки

    // wxLogDebug(wxString::Format("%i %i %i - %i %i %i (%i %i %i)", zIndex, position.x, position.y, marked.z, fromGrid(marked).x, fromGrid(marked).y, marked.z, marked.x, marked.y));

    if (position == fromGrid(marked) && marked.z == zIndex) { // если данный камень выбран, 
        front = wxColor(0xc8, 0xff, 0xc8); // заменяем цвета на аналогичные с зелёным оттенком
        back = wxColor(0xbe, 0xdc, 0xbe);
    }

    dc.SetBrush(back); // устанавливаем кисть для рисования подложки камня

    dc.DrawRoundedRectangle( // рисуем подложку,
        position.x - tilePadding.x * (zIndex - 1), // смещённую относительно плоской позиции на оду единицу смещения вправо и на zIndex единиц влево,
        position.y - tilePadding.y * (zIndex - 1), // на единицу смещения вниз и на zIndex единиц вверх
        tilePixelSize.x * 2, tilePixelSize.y * 2, // размер каждой из сторон в два раза больше, чем размеры на сетке
        tilePadding.y // радиус закругления равен одной единице смещения
    );

    dc.SetBrush(front);

    dc.DrawRoundedRectangle( // рисуем лицевую часть, 
        position.x - tilePadding.x * zIndex, // смещённую относительно плоской позиции на zIndex единиц смещения влево
        position.y - tilePadding.y * zIndex, // и на zIndex единиц вверх
        tilePixelSize.x * 2, tilePixelSize.y * 2, // размер каждой из сторон в два раза больше, чем размеры на сетке
        tilePadding.y // радиус закругления равен одной единице смещения
    );

    dc.SetBrush(_bgColor); // возвращаем изначальный цвет кисти dc

    if (tileImages[index].IsOk()) { // если при загрузке картинки не возникло проблем
        wxPoint pos; // верхний левый угол картинки (так же как у подложки) смещён относительно лицевой части на одну единицу смещения
        pos.x = position.x - tilePadding.x * (zIndex - 1);
        pos.y = position.y - tilePadding.y * (zIndex - 1);

        dc.DrawBitmap( // отрисовываем картинку, масштабируя её под размер камня - две единицы смещения для отступов 
            tileImages[index].Scale(tilePixelSize.x * 2 - tilePadding.x * 2,
                                    tilePixelSize.y * 2 - tilePadding.y * 2),
            pos);
    }
}

void Drawer::resizeBg(const wxSize& resolution) {
    if (this->resolution != resolution) { // если размер окна действительно изменился
        this->resolution = resolution; // устанавливаем новое разрешение и
        composeBG(); // перерисовываем фон
    }
}

/**
 * Resizes tile and whole board bitmap size to the resolution, set in this
 * instance
 */
bool Drawer::resizeBoard(const TLVec& table, const Dimensions& gridSize, bool force) {
    bool res = false; // произошла ли полная перерисовка поля, или только был о рассчитано новое положение битмапа

    const int gridPoint = mmin( // минимум из двух осей (по x и по y), подробнее о формулах в (4)
        resolution.x / (gridSize.x * TILE_WIDTH + gridSize.z * TILE_PADDING_SCALE),
        resolution.y * TILE_WIDTH /
            (gridSize.y * TILE_HEIGHT * TILE_WIDTH + TILE_HEIGHT * gridSize.z * TILE_PADDING_SCALE));

    wxLogDebug(wxString::Format("Resize board: %i", gridPoint));

    if (gridPoint != prevGridPoint || force) { // если gridPoint изменился, или перерасчёт принудительный
        tilePixelSize.Set(gridPoint * TILE_WIDTH, gridPoint * TILE_HEIGHT); // устанавливаем новый размер половины стороны камня (по сетке)

        tilePadding.x = tilePixelSize.x / TILE_WIDTH * TILE_PADDING_SCALE; // Смещение, даваемое подложками карт вдоль оси x
        tilePadding.y = tilePixelSize.y / TILE_WIDTH * TILE_PADDING_SCALE; // Смещение, даваемое подложками карт вдоль оси y

        boardPadding.x = tilePadding.x * (gridSize.z - 1); // Смещение, создаваемое самыми левыми картами на верхних позициях (их может и не быть, но проверять это дорого)
        boardPadding.y = tilePadding.y * (gridSize.z - 1); // Смещение, создаваемое самыми верхними (в плоскости xy) картами на верхних позициях (их может и не быть, но проверять это дорого)

        boardPixelRect.SetWidth(
            (tilePixelSize.x * gridSize.x) + // Размер только плоских карт
            boardPadding.x + // см. выше
            tilePadding.x //  Смещение, даваемое подложками самых правых
        );
        boardPixelRect.SetHeight(
            (tilePixelSize.y * gridSize.y) + // Размер только плоских карт
            boardPadding.y + // см. выше
            tilePadding.y //  Смещение, даваемое подложками самых нижних (в плоскости xy)
        );
    }

    boardPixelRect.SetPosition( // выравниваем по центру окна
        wxPoint((resolution.x - boardPixelRect.width) / 2,
                (resolution.y - boardPixelRect.height) / 2)
    );

    if (gridPoint != prevGridPoint || force) { // если gridPoint изменился, или перерасчёт принудительный
        composeBoard(table, gridSize); // перерисовываем стол
        res = true; // и сообщаем об этом
    }

    prevGridPoint = gridPoint; // сохраняем новое значение gridPoint для "кеширования" стола

    return res;
}

wxPoint Drawer::toGrid(const wxPoint& point) const {
    wxPoint out(-1, -1); // инициализируем координату не валидными значениями

    if (point.x >= boardPixelRect.x + boardPadding.x && // если попали внутрь стола (за исключением декоративных отступов)
        point.x <= boardPixelRect.x + boardPixelRect.width - tilePadding.x &&
        point.y >= boardPixelRect.y + boardPadding.y &&
        point.y <= boardPixelRect.y + boardPixelRect.height - tilePadding.y) {
        out.x = (point.x - boardPixelRect.x - boardPadding.x) / tilePixelSize.x; // перводим курсор в координаты стола (поэтому вычитаем положение битмапа и декоративные отступы)
        out.y = (point.y - boardPixelRect.y - boardPadding.y) / tilePixelSize.y;
    }

    return out; // возвращаем либо заглушку, либо, если попали, валидные координаты
}

/**
 * Converts from grid position to board bitmap coordinates
 */
wxPoint Drawer::fromGrid(int x, int y) const {
    return {x * tilePixelSize.x + boardPadding.x, y * tilePixelSize.y + boardPadding.y}; // переводим из координат сетки в позицию внутри битмапа
}

/**
 * Converts from grid position to board bitmap coordinates
 */
wxPoint Drawer::fromGrid(const wxPoint& point) const {
    return fromGrid(point.x, point.y); // просто проксируем класс wxPoint в функцию с двумя аргументами
}

wxSize Drawer::composeMinSize(const Dimensions& gridSize) const {
    wxSize ms;

    ms.SetWidth(TILE_WIDTH * gridSize.x + gridSize.z * TILE_PADDING_SCALE); // минимальная ширина экрана при gridPoint = 1
    ms.SetHeight(TILE_HEIGHT * gridSize.y + gridSize.z * TILE_PADDING_SCALE * TILE_HEIGHT / TILE_WIDTH); // минимальная высота экрана при gridPoint = 1

    ms += {1, 1}; // добавляем по единице к каждой из сторон, чтобы при округлении не получить число меньше, чем нужно

    wxLogDebug(wxString::Format("MinSize %i %i", ms.x, ms.y));

    return ms;
}
