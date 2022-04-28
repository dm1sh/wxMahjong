#ifndef MainFrame_H_
#define MainFrame_H_

#include "wxw.h"

#include "GamePanel.h"

#include <wx/stdpaths.h>
#include <wx/filename.h>

/// @uml{style[#line.dotted:blue]}
class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    void initMenu();
    void bindMenu();

    void openLayout();

    GamePanel *panel;

    const wxString dataDirPath;
    wxString layoutPath;
};

enum
{
    IDM_Open = wxID_OPEN,
    IDM_Exit = wxID_EXIT,
    IDM_Help = wxID_HELP,
    IDM_About = wxID_ABOUT,
    IDM_Rules = wxID_HIGHEST + 1,
    IDM_New_Game
};

#endif
