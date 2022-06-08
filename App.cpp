#include "App.h"
#include "MainFrame.h"

#include <wx/filefn.h>

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());

    MainFrame* frame = new MainFrame(); // Создаём,

    if (argc >= 2 && wxFileExists(argv[1]))
        frame->layoutPath = argv[1];

    frame->Show(true); // показываем
    SetTopWindow(frame); // и устанавливаем главным окном, а так же выносим вперёд основное окно игры

    return true;
}
