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
    if (solveable)
        controller.fillSolveableTable();
    else
        controller.fillRandom();

    timer->Start(1000, wxTIMER_CONTINUOUS);
    
    if (sb == nullptr)
        sb = ((wxFrame*)this->GetParent())->GetStatusBar();
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
    wxPoint posPlain = drawer.toGrid(ScreenToClient(wxGetMousePosition()));

    ThreePoint pos = {-1, posPlain.x, posPlain.y};

    if (pos.x > -1) {
        auto card = controller.getCardByPosition(pos);

        drawer.marked = pos;

        if (pos.z >= 0 && controller.available(pos))
            controller.select(card);
    }

    Refresh();
}
