#include "./GamePanel.h"

#include <wx/dcbuffer.h>

GamePanel::GamePanel(wxFrame* parent) : wxPanel(parent) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &GamePanel::OnPaint, this);
    Bind(wxEVT_SIZE, [this](wxSizeEvent& evt) -> void {
        this->drawer.tableSize = evt.GetSize();
    });

    timer = new wxTimer(this);
    timerText = drawer.initTimer(this);

    Bind(wxEVT_TIMER, [this](wxTimerEvent& _) -> void {
        controller.stopwatch += 1;
        drawer.drawTimer(timerText, controller.stopwatch);
    }, timer->GetId());
}

void GamePanel::Start() {
    wxLogDebug(_("Started game"));

    controller.stopwatch = 0;
    timer->Start(1000, wxTIMER_CONTINUOUS);

    Refresh();
}

void GamePanel::OnPaint(wxPaintEvent& _) {
    wxAutoBufferedPaintDC dc(this);

    drawer.drawTable(dc);
    drawer.drawTimer(timerText, controller.stopwatch);

    wxLogDebug("Rerender");
}
