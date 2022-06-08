#include "XmlLayout.h"

XmlLayout::XmlLayout() : path("") {}

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
    auto root = layoutDoc.GetRoot();

    lx = wxAtoi(root->GetAttribute("lx"));
    ly = wxAtoi(root->GetAttribute("ly"));

    return {wxAtoi(root->GetAttribute("layers")),
            wxAtoi(root->GetAttribute("ux")) + 2 - lx,
            wxAtoi(root->GetAttribute("uy")) + 2 - ly};
}

void XmlLayout::readLayout(TLVec& table) {
    wxXmlNode* tilePtr = layoutDoc.GetRoot()->GetChildren();

    int x = 0, y = 0, l = 1;

    while (tilePtr) {
        if (tilePtr->GetName().IsSameAs("tile")) {
            x = wxAtoi(tilePtr->GetAttribute("x")) - lx;
            y = wxAtoi(tilePtr->GetAttribute("y")) - ly;
            l = wxAtoi(tilePtr->GetAttribute("layer")) - 1;

            table[l][x][y] = FREE;
        }

        tilePtr = tilePtr->GetNext();
    }
}

uint8_t XmlLayout::getTilesNumber() {
    return wxAtoi(layoutDoc.GetRoot()->GetAttribute("tiles"));
}