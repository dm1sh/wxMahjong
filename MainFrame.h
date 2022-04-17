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
    ~MainFrame();

private:
    void InitMenu();
    void BindMenu();

    void OnOpen(wxCommandEvent& _);

    GamePanel *panel;

    const wxString dataDirPath;
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
