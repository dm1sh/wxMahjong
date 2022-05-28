#include "GamePanel.h"

#include <wx/dcbuffer.h>

#include "events.h"
#include "utils.h"

// clang-format off
wxBEGIN_EVENT_TABLE(GamePanel, wxPanel)
    EVT_PAINT(GamePanel::OnPaint)
    EVT_SIZE(GamePanel::OnResize)
    EVT_TIMER(TIMER_ID, GamePanel::OnTimer)
    EVT_LEFT_DOWN(GamePanel::OnClick)
wxEND_EVENT_TABLE();
// clang-format on

GamePanel::GamePanel(wxFrame* parent)
    : wxPanel(parent), controller(drawer),
      sb(((wxFrame*)this->GetParent())->GetStatusBar()), timer(this, TIMER_ID) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void GamePanel::Start(const wxString& path, bool solveable,
                      std::function<void(const wxSize& size)> setMinSize) {
    wxLogDebug(_("Started game"));

    controller.stopwatch = 0;
    controller.loadLayout(path);
    controller.fill(solveable);

    setMinSize(drawer.composeMinSize(controller.gridSize));

    timer.Start(1000, wxTIMER_CONTINUOUS);

    sb->SetStatusText(LTimeToStr(controller.stopwatch), 0);
    sb->SetStatusText(PRemaining(controller.remaining), 1);

    bool redrawn =
        drawer.resizeBoard(controller.getTable(), controller.gridSize);
    if (!redrawn)
        drawer.composeBoard(controller.getTable(), controller.gridSize);

    Refresh();
}

void GamePanel::undo() {
    controller.undo();

    drawer.composeBoard(controller.getTable(), controller.gridSize);

    Refresh();
}

void GamePanel::reshuffle(bool solveable) {
    controller.free_table();
    controller.fill(solveable);

    drawer.composeBoard(controller.getTable(), controller.gridSize);

    Refresh();
}

void GamePanel::OnPaint(wxPaintEvent& _) {
    wxAutoBufferedPaintDC dc(this);

    wxLogDebug(_("OnPaint"));

    drawer.drawTable(dc);
}

void GamePanel::OnResize(wxSizeEvent& _) {
    const wxSize& resolution = GetClientSize();

    wxLogDebug(wxString::Format("OnResize %i %i", resolution.x, resolution.y));

    if (isPositive(resolution)) {
        drawer.resizeBg(resolution);

        if (controller.gameStarted())
            drawer.resizeBoard(controller.getTable(), controller.gridSize);
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
