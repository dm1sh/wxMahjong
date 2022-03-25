#include "./Drawer.h"

static const char* tileImageNames[] = { "Back", "Blank", "Chun", "Front", "Haku", "Hatsu", "Man1", "Man2", "Man3", "Man4", "Man5", "Man5-Dora", "Man6", "Man7", "Man8", "Man9", "Nan", "Pei", "Pin1", "Pin2", "Pin3", "Pin4", "Pin5", "Pin5-Dora", "Pin6", "Pin7", "Pin8", "Pin9", "Shaa", "Sou1", "Sou2", "Sou3", "Sou4", "Sou5", "Sou5-Dora", "Sou6", "Sou7", "Sou8", "Sou9", "Ton" };

Drawer::Drawer() {
    for (int i = 0; i < 40; i++) {
        tileImages[i].LoadFile(_("./resources/tiles/") + _(tileImageNames[i]) + _(".png"), wxBITMAP_TYPE_PNG);
        tileImages[i].Rescale(60, 80);
    }
}

void Drawer::drawTable(wxDC& dc) {
    drawBG(dc);
    drawTiles(dc);
}

void Drawer::drawBG(wxDC& dc) {
    dc.GradientFillConcentric(wxRect(wxPoint(0, 0), tableSize), wxColor(7, 85, 45), wxColor(1, 45, 22));
}

void Drawer::drawTiles(wxDC& dc) {
    dc.DrawBitmap(tileImages[3], wxPoint(10, 10));
    dc.DrawBitmap(tileImages[10], wxPoint(10, 10));

    dc.DrawBitmap(tileImages[3], wxPoint(80, 10));
    dc.DrawBitmap(tileImages[10], wxPoint(80, 10));
}

wxString LTimeToStr(int time) {
    return wxString::Format(_("%d:%02d:%02d"), time / 3600, (time / 60) % 60, time % 60);
}

void Drawer::drawTimer(wxStaticText* timerText, int time) {
    wxString timeStr = LTimeToStr(time);
    timerText->SetPosition(wxPoint(tableSize.x / 2, 10));
    timerText->SetLabel(timeStr);
}

wxStaticText* Drawer::initTimer(wxWindow* parent) {
    auto timerText = new wxStaticText(parent, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    timerText->SetForegroundColour(wxColor(255, 255, 255));
    timerText->SetFont(wxFont(wxFontInfo(20)));

    return timerText;
}
