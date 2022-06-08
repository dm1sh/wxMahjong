#include "GamePanel.h"

#include <wx/dcbuffer.h>

#include "events.h"
#include "utils.h"

/**
 * Объявляем таблицу обработчиков событий
 */
// clang-format off
wxBEGIN_EVENT_TABLE(GamePanel, wxPanel)
    EVT_PAINT(GamePanel::OnPaint)
    EVT_SIZE(GamePanel::OnResize)
    EVT_TIMER(TIMER_ID, GamePanel::OnTimer)
    EVT_LEFT_DOWN(GamePanel::OnClick)
wxEND_EVENT_TABLE();
// clang-format on

GamePanel::GamePanel(wxFrame* parent)
    : wxPanel(parent), controller(drawer), // вызываем родительский конструктор и передаём drawer в конструктор
      sb(((wxFrame*)this->GetParent())->GetStatusBar()), timer(this, TIMER_ID) { // инициализируем указатель на статус бар окна, создаём таймер
    SetBackgroundStyle(wxBG_STYLE_PAINT); // Устанавливаем wxBG_STYLE_PAINT для того, чтобы при вызове OnPaint не мерцало окно
}

void GamePanel::Start(const wxString& path, bool solvable,
                      std::function<void(const wxSize& size)> setMinSize) {
    wxLogDebug(_("Started game")); // здесь и далее - сообщения, которые выводятся в консоль в дебаг-версии

    controller.stopwatch = 0; // сбрасываем таймер
    controller.loadLayout(path); // загружаем в контроллер схему
    controller.fill(solvable); // заполняем игровое поле

    setMinSize(drawer.composeMinSize(controller.gridSize)); // устанавливаем минимальный размер окна

    timer.Start(1000, wxTIMER_CONTINUOUS); // Запускаем таймер для вызова события каждую секунду

    sb->SetStatusText(LTimeToStr(controller.stopwatch), 0); // В первую колонку статус бара пишем время игры,
    sb->SetStatusText(PRemaining(controller.remaining), 1); // во вторую процент оставшихся камней

    bool redrawn = 
        drawer.resizeBoard(controller.getTable(), controller.gridSize); // Изменяем размер доски
    if (!redrawn) // и если при этом изменился размер камней, 
        drawer.composeBoard(controller.getTable(), controller.gridSize); // перерисовываем доску

    Refresh(); // вызываем перерисовку окна
}

void GamePanel::undo() {
    controller.undo();

    drawer.composeBoard(controller.getTable(), controller.gridSize);

    Refresh();
}

void GamePanel::reshuffle(bool solvable) {
    controller.free_table(); // очищаем стол в контроллере
    controller.fill(solvable); // заполняем его заново

    drawer.composeBoard(controller.getTable(), controller.gridSize); // перерисовываем стол

    Refresh();
}

void GamePanel::OnPaint(wxPaintEvent& _) {
    wxAutoBufferedPaintDC dc(this); // создаём контекст с буфером для предотвращения мерцания

    wxLogDebug(_("OnPaint"));

    drawer.drawTable(dc); // отрисовываем в нём кадр
}

void GamePanel::OnResize(wxSizeEvent& _) {
    const wxSize& resolution = GetClientSize(); // получаем размер клиентской части окна (без рамок, тулбара и статусбара)

    wxLogDebug(wxString::Format("OnResize %i %i", resolution.x, resolution.y));

    if (isPositive(resolution)) { // на некоторых платформах первоначальный размер экрана может установиться в 0, поэтому лучше проверять это
        drawer.resizeBg(resolution); // изменяем размер фона

        if (controller.gameStarted()) // если уже начали игру
            drawer.resizeBoard(controller.getTable(), controller.gridSize); // перерисовываем доску
    }

    Refresh();
}

wxDEFINE_EVENT(END_EVT, wxCommandEvent);

void GamePanel::OnTimer(wxTimerEvent& _) {
    controller.stopwatch += 1;
    sb->SetStatusText(LTimeToStr(controller.stopwatch), 0);

    if (controller.remaining == 0) {
        wxCommandEvent event(END_EVT);
        event.SetString(LTimeToStr(controller.stopwatch));
        wxPostEvent(GetParent(), event);

        timer.Stop();
        controller.stopwatch = 0;
    }
}

void GamePanel::OnClick(wxMouseEvent& _) {
    if (controller.gameStarted()) {
        controller.handleClick(ScreenToClient(wxGetMousePosition()));
        sb->SetStatusText(PRemaining(controller.remaining), 1);

        drawer.composeBoard(controller.getTable(), controller.gridSize);

        wxLogDebug(wxString::Format(_("Remaining %i"), controller.remaining));

        Refresh();
    }
}
