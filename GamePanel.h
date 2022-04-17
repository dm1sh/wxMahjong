#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "wxw.h"

#include <wx/stopwatch.h>

#include "Drawer.h"
#include "Controller.h"

class GamePanel : public wxPanel {
public:
    GamePanel(wxFrame* parent);

    void Start();

private:
    Drawer drawer;
    Controller controller;

    void OnPaint(wxPaintEvent& _);
    void OnTimer(wxTimerEvent& _);
    void OnClick(wxMouseEvent& _);
    
    wxStatusBar* sb;
    wxTimer* timer;
};

#endif
