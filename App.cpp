#include "./App.h"
#include "./GameFrame.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    GameFrame *frame = new GameFrame();
    frame->Show(true);
    SetTopWindow(frame);
    return true;
}
