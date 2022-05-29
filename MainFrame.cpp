#include "MainFrame.h"

#include "AboutDlg.h"
#include "HelpDlg.h"
#include "RulesDlg.h"

#include "events.h"

#include "resources/icon.xpm"

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, _("Маджонг (пасьянс)"), wxDefaultPosition,
              wxSize(800, 600)),
      dataDirPath(wxStandardPaths::Get().GetUserDataDir()) {
    SetIcon(logo_icon);

    initMenu();
    bindMenu();

    Bind(wxEVT_SHOW, [this](wxShowEvent& _) -> void {
        if (openLayout())
            panel->Start(layoutPath, solveable,
                         [this](const wxSize& size) -> void {
                             this->SetMinClientSize(size);
                         });
    });

    Bind(END_EVT, [this](wxCommandEvent& evt) -> void {
        wxMessageDialog dlg(this, _("Хотите сыграть снова?"),
                            _("Игра окончена"), wxYES_NO);
        dlg.SetExtendedMessage(_("Поздравляем, вы закончили игру за ") +
                               evt.GetString());
        if (dlg.ShowModal() == wxID_YES) {
            panel->Start(layoutPath, solveable,
                         [this](const wxSize& size) -> void {
                             this->SetMinClientSize(size);
                         });
        }
    });

    CreateStatusBar(2);

    panel = new GamePanel(this);
    panel->SetFocus();
}

void MainFrame::initMenu() {
    wxMenu* menuGame = new wxMenu;
    menuGame->Append(IDM_New_Game, _("Начать сначала"));
    menuGame->Append(IDM_Open, _("Открыть карту"));
    menuGame->AppendCheckItem(IDM_Solveable, _("Генерировать решаемую карту"));
    menuGame->AppendSeparator();
    menuGame->Append(IDM_Undo, _("Отменить ход"));
    menuGame->Append(IDM_Reshuffle, _("Перемешать поле"));
    menuGame->AppendSeparator();
    menuGame->Append(IDM_Exit, _("Выход"));

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(IDM_Help, _("Инструкция"));
    menuHelp->Append(IDM_Rules, _("Правила игры"));
    menuHelp->Append(IDM_About, _("О программе"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuGame, _("Игра"));
    menuBar->Append(menuHelp, _("Помощь"));

    SetMenuBar(menuBar);
}

void MainFrame::bindMenu() {
    Bind(
        wxEVT_MENU, [this](wxCommandEvent& _) -> void { Close(); }, IDM_Exit);

    Bind(
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            if (openLayout())
                panel->Start(layoutPath, solveable,
                             [this](const wxSize& size) -> void {
                                 this->SetMinClientSize(size);
                             });
        },
        IDM_Open);

    Bind(
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            (new HelpDlg(this, wxID_ANY))->Show();
        },
        IDM_Help);

    Bind(
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            (new AboutDlg(this, wxID_ANY))->Show();
        },
        IDM_About);

    Bind(
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            (new RulesDlg(this, wxID_ANY))->Show();
        },
        IDM_Rules);

    Bind(
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            if (!layoutPath.IsEmpty() || openLayout()) {
                panel->Start(layoutPath, solveable,
                             [this](const wxSize& size) -> void {
                                 this->SetMinClientSize(size);
                             });
            }
        },
        IDM_New_Game);

    Bind(
        wxEVT_MENU,
        [this](wxCommandEvent& evt) -> void { solveable = evt.IsChecked(); },
        IDM_Solveable);

    Bind(
        wxEVT_MENU, [this](wxCommandEvent& _) -> void { panel->undo(); },
        IDM_Undo);

    Bind(
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void { panel->reshuffle(solveable); },
        IDM_Reshuffle);
}

/**
 * Shows a file opening dialog asking for .smlf file if succed, sets its path to
 * layoutPath
 * @return true if user have chosen a file, false if cancelled
 */
bool MainFrame::openLayout() {
    wxFileDialog openFileDlg(
        this, _("Открыть карту"),
        dataDirPath + wxFileName::GetPathSeparator() + _("layouts"),
        _("Turtle.smlf"), _("Файлы Mahjong карт (*.smlf)|*.smlf"),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDlg.ShowModal() == wxID_CANCEL)
        return false;

    layoutPath = openFileDlg.GetPath();
    return true;
}
