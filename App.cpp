#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());

    MainFrame* frame = new MainFrame();
    frame->Show(true);
    SetTopWindow(frame);

    return true;
}
