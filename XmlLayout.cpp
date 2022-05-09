#include "XmlLayout.h"

XmlLayout::XmlLayout() : path("") { }

bool XmlLayout::openFile(const wxString& openPath) {
    if (openPath.IsSameAs(path))
        return true;

    if (!layoutDoc.Load(openPath))
        return false;

    if (layoutDoc.GetRoot() == nullptr)
        return false;

    return true;
}

Dimensions XmlLayout::getDimensions() {
    return { wxAtoi(layoutDoc.GetRoot()->GetAttribute("layers")),
            wxAtoi(layoutDoc.GetRoot()->GetAttribute("ux")) + 2,
            wxAtoi(layoutDoc.GetRoot()->GetAttribute("uy")) + 2 };
}

void XmlLayout::readLayout(TLVec& table) {
    wxXmlNode* tilePtr = layoutDoc.GetRoot()->GetChildren();

    int x = 0, y = 0, l = 1;

    while (tilePtr) {
        if (tilePtr->GetName().IsSameAs("tile")) {
            x = wxAtoi(tilePtr->GetAttribute("x"));
            y = wxAtoi(tilePtr->GetAttribute("y"));
            l = wxAtoi(tilePtr->GetAttribute("layer")) - 1;

            table[l][x][y] = -1;
        }

        tilePtr = tilePtr->GetNext();
    }
}

uint8_t XmlLayout::getTilesNumber() {
    return wxAtoi(layoutDoc.GetRoot()->GetAttribute("tiles"));
}