#include "GamePanel.h"

#include <wx/dcbuffer.h>

#include "utils.h"

GamePanel::GamePanel(wxFrame* parent) : wxPanel(parent), controller(drawer) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &GamePanel::OnPaint, this);
    Bind(wxEVT_SIZE, [this](wxSizeEvent& evt) -> void {
        this->controller.resize(evt.GetSize());
    });

    timer = new wxTimer(this, 1);
    Bind(wxEVT_TIMER, &GamePanel::OnTimer, this, timer->GetId());

    Bind(wxEVT_LEFT_DOWN, &GamePanel::OnClick, this);
}

void GamePanel::Start(const wxString& path, bool solveable) {
    controller.stopwatch = 0;
    controller.loadLayout(path);
    controller.fill(solveable);

    timer->Start(1000, wxTIMER_CONTINUOUS);
    
    if (sb == nullptr)
        sb = ((wxFrame*)this->GetParent())->GetStatusBar();
    sb->SetStatusText(LTimeToStr(controller.stopwatch), 0);
    sb->SetStatusText(PRemaining(controller.remaining), 1);

    drawer.initScreen(controller.getTable());
}

void GamePanel::OnPaint(wxPaintEvent& _) {
    wxAutoBufferedPaintDC dc(this);

    drawer.drawTable(dc);
}

void GamePanel::OnTimer(wxTimerEvent& _) {
    controller.stopwatch += 1;
    sb->SetStatusText(LTimeToStr(controller.stopwatch), 0);
}

void GamePanel::OnClick(wxMouseEvent& _) {
    controller.handleClick(ScreenToClient(wxGetMousePosition()));
    sb->SetStatusText(PRemaining(controller.remaining), 1);

    Refresh();
}

void GamePanel::undo() {
    controller.undo();

    drawer.initScreen(controller.getTable());

    Refresh();
}

void GamePanel::reshuffle(bool solveable) {
    controller.free_table();
    controller.fill(solveable);
    drawer.initScreen(controller.getTable());

    Refresh();
}
