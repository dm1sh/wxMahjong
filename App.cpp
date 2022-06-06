#include "App.h"
#include "MainFrame.h"

#include <wx/filefn.h>

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());

    MainFrame* frame = new MainFrame();

    if (argc >= 2 && wxFileExists(argv[1]))
        frame->layoutPath = argv[1];

    frame->Show(true);
    SetTopWindow(frame);

    return true;
}
