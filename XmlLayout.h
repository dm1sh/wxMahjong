#ifndef XMLLAYOUT_H
#define XMLLAYOUT_H

#include "wxw.h"

#include <wx/xml/xml.h>

#include "utils.h"

class XmlLayout {
public:
    XmlLayout();

    bool openFile(const wxString& path);
    Dimensions getDimensions();
    void readLayout(TLVec& table);
    uint8_t getTilesNumber();

private:
    wxString path;

    int lx;
    int ly;

    wxXmlDocument layoutDoc;
};

#endif
