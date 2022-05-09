#include "MainFrame.h"

#include "HelpDlg.h"
#include "RulesDlg.h"
#include "AboutDlg.h"

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, _("Маджонг (пасьянс)"), wxDefaultPosition, wxSize(800, 600)),
    dataDirPath(wxStandardPaths::Get().GetUserDataDir())
{
    initMenu();
    bindMenu();

    CreateStatusBar();

    panel = new GamePanel(this);
    panel->SetFocus();

    openLayout();
}

void MainFrame::initMenu() {
    wxMenu *menuGame = new wxMenu;
    menuGame->Append(IDM_New_Game, _("Начать сначала"));
    menuGame->Append(IDM_Open, _("Открыть карту"));
    menuGame->AppendCheckItem(IDM_Solveable, _("Генерировать решаемую карту"));
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

void MainFrame::bindMenu() {
    Bind(wxEVT_MENU, [this](wxCommandEvent& _) -> void {
        Close();
    }, IDM_Exit);

    Bind(wxEVT_MENU, [this](wxCommandEvent& _) -> void {
        openLayout();
    }, IDM_Open);

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
        if (layoutPath.IsEmpty())
            openLayout();
        else
            panel->Start(layoutPath, solveable);
            
        Refresh();
    }, IDM_New_Game);

    Bind(wxEVT_MENU, [this](wxCommandEvent& _) -> void {
        solveable = _.IsChecked();
    }, IDM_Solveable);
}

void MainFrame::openLayout() {
    wxFileDialog openFileDlg(this, "Открыть карту", dataDirPath + wxFileName::GetPathSeparator() + _("layouts"), "Turtle.smlf", "Файлы Mahjong карт (*.smlf)|*.smlf", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDlg.ShowModal() == wxID_CANCEL)
        return;

    layoutPath = openFileDlg.GetPath();

    panel->Start(layoutPath, solveable);
}
