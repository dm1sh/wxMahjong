#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "wxw.h"

#include <wx/stopwatch.h>

#include "Controller.h"
#include "Drawer.h"

class GamePanel : public wxPanel {
public:
    GamePanel(wxFrame* parent);

    void Start(const wxString& path, bool solveable,
               std::function<void(const wxSize& size)> setMinSize);

    void undo();
    void reshuffle(bool solveable);

private:
    Drawer drawer;
    Controller controller;

    void OnPaint(wxPaintEvent& _);
    void OnResize(wxSizeEvent& _);
    void OnTimer(wxTimerEvent& _);
    void OnClick(wxMouseEvent& _);

    wxDECLARE_EVENT_TABLE();

    wxStatusBar* sb = nullptr;
    wxTimer timer;
};

#define TIMER_ID 1

#endif
