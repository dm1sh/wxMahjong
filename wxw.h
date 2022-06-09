#ifndef WXW_H_
#define WXW_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// переопределяем макрос для преобразования C-строки в UTF-8 wxString
#ifdef _
#undef _
#endif
#define _(s) wxString::FromUTF8(s)

#endif
