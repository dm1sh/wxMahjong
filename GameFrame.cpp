#include "./GameFrame.h"

#include "./HelpDlg.h"
#include "./RulesDlg.h"
#include "./AboutDlg.h"

BEGIN_EVENT_TABLE(GameFrame, wxFrame)
    EVT_MENU(IDM_Exit, GameFrame::OnExit)
    EVT_MENU(IDM_Help, GameFrame::OnHelp)
    EVT_MENU(IDM_Rules, GameFrame::OnRules)
    EVT_MENU(IDM_About, GameFrame::OnHelp)
    EVT_PAINT(GameFrame::OnPaint)
END_EVENT_TABLE()

GameFrame::GameFrame()
    : wxFrame(nullptr, wxID_ANY, _("Маджонг (пасьянс)"))
{
    InitMenu();
}

void GameFrame::InitMenu() {
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

void GameFrame::OnExit(wxCommandEvent &event) {
    Close();
}

void GameFrame::OnHelp(wxCommandEvent &event) {
    HelpDlg *dlg = new HelpDlg(this, -1);

    dlg->Show();
}

void GameFrame::OnRules(wxCommandEvent &event) {
    RulesDlg *dlg = new RulesDlg(this, -1);

    dlg->Show();
}

void GameFrame::OnAbout(wxCommandEvent &event) {
    AboutDlg *dlg = new AboutDlg(this, -1);

    dlg->Show();
}

void GameFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);

    paint.drawTable(dc, GetSize());
}
