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

wxSize XmlLayout::getDimensions() {
    return { wxAtoi(layoutDoc.GetRoot()->GetAttribute("ux")) + 2,
            wxAtoi(layoutDoc.GetRoot()->GetAttribute("uy")) + 2 };
}

void XmlLayout::readLayout(TLVec& table) {
    wxXmlNode* tilePtr = layoutDoc.GetRoot()->GetChildren();

    int x = 0, y = 0, l = 1;

    while (tilePtr) {
        if (tilePtr->GetName().IsSameAs("tile")) {
            x = wxAtoi(tilePtr->GetAttribute("x"));
            y = wxAtoi(tilePtr->GetAttribute("y"));
            l = wxAtoi(tilePtr->GetAttribute("layer"));

            table[x][y].push_back(std::make_pair(l - 1, (uint8_t)-1));

            table[x][y].at(table[x][y].size() -1).second = (random()) % 37;
        }

        tilePtr = tilePtr->GetNext();
    }
}
