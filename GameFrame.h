#ifndef GAMEFRAME_H_
#define GAMEFRAME_H_

#include "./wxw.h"

#include "./Graphics.h"

/// @uml{style[#line.dotted:blue]}
class GameFrame : public wxFrame
{
public:
    GameFrame();

private:
    Graphics paint;

    void InitMenu();

    void OnExit(wxCommandEvent &event);
    void OnHelp(wxCommandEvent &event);
    void OnRules(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    void OnPaint(wxPaintEvent& event);

    DECLARE_EVENT_TABLE()
};

enum
{
    IDM_Exit = wxID_EXIT,
    IDM_Help = wxID_HELP,
    IDM_About = wxID_ABOUT,
    IDM_New_Game = 1,
    IDM_Rules
};

#endif
