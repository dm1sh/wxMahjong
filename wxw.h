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

#endif
