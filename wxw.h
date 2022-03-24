#ifndef WXW_H_
#define WXW_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#ifdef _
#undef _
#endif
#define _(s) wxString::FromUTF8(s)

// class wxTrackable {};
// class wxObject {};
// class wxEventHandler : wxObject, wxTrackable {};
// class wxEventFilter {};
// class wxAppConsole : wxEventHandler, wxEventFilter {};
// class wxApp : wxAppConsole {};

// class wxWindow : wxEventHandler {};
// class wxNotOwnedWindow : wxWindow {};
// class wxTopLevelWindow : wxNotOwnedWindow {};
// class wxFrame : wxTopLevelWindow {};

// class wxCommandEvent {};

// #define DECLARE_EVENT_TABLE() void none();

#endif
