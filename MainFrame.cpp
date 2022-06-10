#include "MainFrame.h"

#include "TextDlg.h"
#include "TXTContents.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "events.h"

#include "resources/icon.xpm"

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, _("Маджонг (пасьянс)"), wxDefaultPosition, // указываем то, что у этого окна нет родителя, оно может иметь любой id, так же устанавливаем заголовок и позицию на усмотрение оконного менеджера
              wxSize(800, 600)), // устанавливаем стандартный размер окна
      dataDirPath(wxStandardPaths::Get().GetUserDataDir()), // сохраняем стандартный путь до ресурсов программы
      setMinSize_fn{[this](const wxSize& size) -> void { // создаём лямбду с замыканием внутри неё методов для установки минимального размера окна и передачи её как аргумент в метод другого класса
          this->SetMinClientSize(size); // устанавливаем минимальный размер окна для оконного менеджера
          const auto& curr = this->GetClientSize(); // считываем нынешний размер окна 
          this->SetClientSize({mmax(size.x, curr.x), mmax(size.y, curr.y)}); // если нынешний размер окна меньше, чем требует карта, увеличиваем ту сторону окна до минимальной необходимой
      }} {
    SetIcon(logo_icon);

    initMenu(); // Создание пунктов меню
    bindMenu(); // Подключение обработчиков пунктов меню

    Bind(wxEVT_SHOW, [this](wxShowEvent& _) -> void { // обработчик события отображения окна
        if (!layoutPath.IsEmpty() || openLayout()) // если пользователь выбрал схему
            startGame();
    });

    Bind(END_EVT, [this](wxCommandEvent& evt) -> void { // обработчик кастомного события окончания игры
        wxMessageDialog dlg(this, _("Хотите сыграть снова?"),
                            _("Игра окончена"), wxYES_NO); // Создаём диалог с предложением начать игру заново 
        dlg.SetExtendedMessage(_("Поздравляем, вы закончили игру за ") +
                               evt.GetString()); // Устанавливаем время, затраченное на полное выполнение карты
        if (dlg.ShowModal() == wxID_YES) // Если пользователь хочет,
            startGame(); // начинаме игру заново
    });

    CreateStatusBar(2); // Создаём статусбар с двумя колонками

    panel = new GamePanel(this); // Создаём
    panel->SetFocus(); // и показываем панель, где отрисовывается игровое поле
}

void MainFrame::initMenu() {
    wxMenu* menuGame = new wxMenu; // Создаём подменю
    menuGame->Append(IDM_New_Game, _("Начать сначала\tCTRL+N")); // Создаем пункт меню с id обработчика IDM_New_Game, далее аналогично
    menuGame->Append(IDM_Open, _("Открыть карту\tCTRL+O"));
    menuGame->AppendCheckItem(IDM_Solveable, _("Генерировать решаемую карту"));
    menuGame->AppendSeparator(); // Добавляем горизонтальный разделитель в меню
    menuGame->Append(IDM_Undo, _("Отменить ход"));
    menuGame->Append(IDM_Reshuffle, _("Перемешать поле"));
    menuGame->AppendSeparator();
    menuGame->Append(IDM_Exit, _("Выход\tCTRL+Q"));

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(IDM_Help, _("Инструкция"));
    menuHelp->Append(IDM_Rules, _("Правила игры"));
    menuHelp->Append(IDM_About, _("О программе"));

    wxMenuBar* menuBar = new wxMenuBar; // Создаём меню бар,
    menuBar->Append(menuGame, _("Игра")); // куда подключаем созданные выше подменю
    menuBar->Append(menuHelp, _("Помощь"));

    SetMenuBar(menuBar); // И устанавливаем его как основной для этой панели
}

void MainFrame::bindMenu() {
    Bind(
        wxEVT_MENU, [this](wxCommandEvent& _) -> void { Close(); }, IDM_Exit); // Вешаем обработчик закртытия игры при выборе соответствующего пункта меню

    Bind( // Вешаем обработчик для открытия схемы и запуска соответствующей игры
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            if (openLayout()) // запрашиваем в диалоге путь до файла карты и если получаем,
                startGame(); // запускаем игру
        },
        IDM_Open);

    Bind( // Вешаем обработчик для открытия диалога с "помощью"
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            (new TextDlg(this, wxID_ANY, _("Помощь"), TXTContents::help))->Show();
        },
        IDM_Help);

    Bind( // Вешаем обработчик для открытия диалога с "правилами"
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            (new TextDlg(this, wxID_ANY, _("Правила игры"), TXTContents::rules))->Show();
        },
        IDM_Rules);

    Bind( // Вешаем обработчик для открытия диалога "о программе"
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            (new TextDlg(this, wxID_ANY, _("О программе"), TXTContents::about))->Show();
        },
        IDM_About);

    Bind( // Вешаем обработчик для запуска новой игры
        wxEVT_MENU,
        [this](wxCommandEvent& _) -> void {
            if (!layoutPath.IsEmpty() || openLayout()) // если ещё не выбран файл с картой, открываем диалог для его выбора
                startGame(); // если путь был, или открыт в диалоге, начинаем игру
        },
        IDM_New_Game);

    Bind( // Вешаем обработчик для установки режима генерации поля
        wxEVT_MENU,
        [this](wxCommandEvent& evt) -> void { solveable = evt.IsChecked(); }, // устанавливаем флаг способа генерации поля согласно значению чекбокса в меню
        IDM_Solveable);

    Bind( // Вешаем обработчик для отмены хода
        wxEVT_MENU, [this](wxCommandEvent& _) -> void { panel->undo(); },
        IDM_Undo);

    Bind( // Вешаем обработчик для перемешивания поля
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
    wxFileDialog openFileDlg( // Создаём диалог для запроса файла схемы
        this, _("Открыть карту"),
        dataDirPath + wxFileName::GetPathSeparator() + _("layouts"), // Стандартный путь до файлов приложения
        _("Turtle.smlf"), _("Файлы Mahjong карт (*.smlf)|*.smlf"), // Стандартный файл и поддерживаемые форматы файлов
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDlg.ShowModal() == wxID_CANCEL) // если пользователь не выбрал файл
        return false; // возвращаем false, т.к. открытие карты не успешно

    layoutPath = openFileDlg.GetPath(); // сохраняем путь до файла 
    return true;
}

void MainFrame::startGame() {
    panel->Start(layoutPath, solveable, // запускаем игру с путём до схемы, выбранным режимом заполнения поля
                         setMinSize_fn // и проброшенной функцией установки минимального размера окна
                         );
}
