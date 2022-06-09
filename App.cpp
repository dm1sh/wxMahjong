#include "App.h"
#include "MainFrame.h"

#include <wx/filefn.h>

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());

    MainFrame* frame = new MainFrame(); // Создаём окно игры

    if (argc >= 2 && wxFileExists(argv[1])) // Если пользователь ввёл какие-то аргументы
        frame->layoutPath = argv[1]; // считаем, что первый аргумент - путь до файла карты

    frame->Show(true); // показываем окно
    SetTopWindow(frame); // и устанавливаем главным, а так же выносим вперёд

    return true;
}
