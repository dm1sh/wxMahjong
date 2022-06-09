#ifndef TEXTDLG_H
#define TEXTDLG_H

#include "wxw.h"

class TextDlg : public wxDialog {
public:
    TextDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxString& content);
};

#endif
