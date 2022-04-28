#ifndef XMLLAYOUT_H
#define XMLLAYOUT_H

#include "wxw.h"

#include <wx/xml/xml.h>

#include "utils.h"

class XmlLayout {
public:
    XmlLayout();

    bool openFile(const wxString& path);
    wxSize getDimensions();
    void readLayout(TLVec& table);

private:
    wxString path;

    wxXmlDocument layoutDoc;
};

#endif
