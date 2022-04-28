#include "GamePanel.h"

#include <wx/dcbuffer.h>

#include "utils.h"

GamePanel::GamePanel(wxFrame* parent) : wxPanel(parent), controller(drawer), sb(parent->GetStatusBar()) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &GamePanel::OnPaint, this);
    Bind(wxEVT_SIZE, [this](wxSizeEvent& evt) -> void {
        this->controller.resize(evt.GetSize());
    });

    timer = new wxTimer(this);
    Bind(wxEVT_TIMER, &GamePanel::OnTimer, this, timer->GetId());

    Bind(wxEVT_LEFT_DOWN, &GamePanel::OnClick, this);
}

void GamePanel::Start(const wxString& path) {
    controller.stopwatch = 0;
    controller.loadLayout(path);

    timer->Start(1000, wxTIMER_CONTINUOUS);
    sb->SetStatusText(LTimeToStr(controller.stopwatch));
}

void GamePanel::OnPaint(wxPaintEvent& _) {
    wxAutoBufferedPaintDC dc(this);

    drawer.drawTable(dc);
}

void GamePanel::OnTimer(wxTimerEvent& _) {
    controller.stopwatch += 1;
    sb->SetStatusText(LTimeToStr(controller.stopwatch));
}

void GamePanel::OnClick(wxMouseEvent& _) {
    wxPoint res = controller.toGrid(ScreenToClient(wxGetMousePosition()));

    if (res.x > -1) {
        auto card = controller.getCardByPosition(res);
        if (card != nullptr) {
            controller.select(card);
        }
    }

    Refresh();
}
