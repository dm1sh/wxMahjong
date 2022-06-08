#ifndef MainFrame_H_
#define MainFrame_H_

#include "wxw.h"

#include "GamePanel.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>

class MainFrame : public wxFrame {
public:
    MainFrame();

    wxString layoutPath;

private:
    void initMenu();
    void bindMenu();

    GamePanel* panel;

    bool openLayout();
    void startGame();

    const std::function<void(const wxSize&)> setMinSize_fn =
        [this](const wxSize& size) -> void { this->SetMinClientSize(size); };

    const wxString dataDirPath;

    bool solveable = false; // determites wether to generate solveable or
                            // completely random map
};

enum {
    IDM_Open = wxID_OPEN,
    IDM_Exit = wxID_EXIT,
    IDM_Help = wxID_HELP,
    IDM_About = wxID_ABOUT,
    IDM_Rules = wxID_HIGHEST + 1,
    IDM_New_Game,
    IDM_Solveable,
    IDM_Undo,
    IDM_Reshuffle
};

#endif
