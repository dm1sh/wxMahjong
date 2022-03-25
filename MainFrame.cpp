#include "./MainFrame.h"

#include "./HelpDlg.h"
#include "./RulesDlg.h"
#include "./AboutDlg.h"

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, _("Маджонг (пасьянс)"))
{
    InitMenu();
    BindMenu();

    panel = new GamePanel(this);
    panel->SetFocus();
    panel->Start();
}

void MainFrame::InitMenu() {
    wxMenu *menuGame = new wxMenu;
    menuGame->Append(IDM_New_Game, _("Начать сначала"));
    menuGame->AppendSeparator();
    menuGame->Append(IDM_Exit, _("Выход"));

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(IDM_Help, _("Инструкция"));
    menuHelp->Append(IDM_Rules, _("Правила игры"));
    menuHelp->Append(IDM_About, _("О программе"));

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuGame, _("Игра"));
    menuBar->Append(menuHelp, _("Помощь"));

    SetMenuBar(menuBar);
}

void MainFrame::BindMenu() {
    Bind(wxEVT_MENU, [this](wxCommandEvent& _) -> void {
        Close();
    }, IDM_Exit);

    Bind(wxEVT_MENU, [this](wxCommandEvent& _) -> void {
        (new HelpDlg(this, -1))->Show();
    }, IDM_Help);

    Bind(wxEVT_MENU, [this](wxCommandEvent& _) -> void {
        (new AboutDlg(this, -1))->Show();
    }, IDM_About);

    Bind(wxEVT_MENU, [this](wxCommandEvent& _) -> void {
        (new RulesDlg(this, -1))->Show();
    }, IDM_Rules);

    Bind(wxEVT_MENU, [this](wxCommandEvent& _) -> void {
        panel->Start();
    }, IDM_New_Game);
}
