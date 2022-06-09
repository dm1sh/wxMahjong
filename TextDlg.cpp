#include "TextDlg.h"

#include <wx/settings.h>

#include "utils.h"

TextDlg::TextDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxString& content)
    : wxDialog::wxDialog(parent, id, title) {

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL); // создаём сайзер для окна прокрутки
    SetSizer(sizer); // устанавливаем его как главный сайзер окна

    wxScrolledWindow* scrollableWnd = new wxScrolledWindow( // создаём окно прокрутки
        this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL); // указываем стандартные параметры и говорим, что прокрутка должна быть только вертикальная

    sizer->Add(scrollableWnd, 1, wxGROW | wxALL, 5); // добавляем в сайзер окно прокрутки с отступами по краям в 5 пикселей

    wxStaticText* text = // внутри окна прокрутки создаём статический текст, содержащий `content`, поддерживающий перенос строки
        new wxStaticText(scrollableWnd, wxID_ANY, content, wxDefaultPosition,
                         wxDefaultSize, wxSP_WRAP);

    const wxClientDC dc(text); // создаём dc, используя для него настройки статического текста
    const wxSize lineSize = dc.GetTextExtent(wxString('W', 40U)); // получаем из dc размеры строчки из 40 символов 'W' (так как текст удобнее всего читать, если в нём около 40-60 символов в строке)

    scrollableWnd->SetScrollbars(lineSize.x, lineSize.y, 0, 0); // устанавливаем скорость скролла (количество пикселей, прокручиваемых при одинарном прокручивании колеса мыши, или нажатие кнопки)

    text->Wrap(lineSize.x); // Делаем перенос строк для того, чтобы вместить текст в ширину строки

    auto textSize = text->GetClientSize(); // получаем размер статического текста

    scrollableWnd->SetVirtualSize(textSize.x, textSize.y); // устанавливаем виртуальный размер окна прокрутки равным размерам статического текста

    SetClientSize( // размер видимого окна устанавливаем равным
        textSize.x + 10 + wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y), // по ширине: ширина текста
        mmin(textSize.y, lineSize.y * 30) + 10); // по высоте: минимум из реальной высоты текста, или 30 строк
        // + отступы по 5 пикселей с обеих сторон
}