#include "XmlLayout.h"

XmlLayout::XmlLayout() : path("") {}

bool XmlLayout::openFile(const wxString& openPath) {
    if (openPath.IsSameAs(path) && layoutDoc.Load(openPath)) // если открыли ту же карту, что и в прошлый раз и она открывалась
        return true; // можно не читать заново

    if (!layoutDoc.Load(openPath)) // если возникла ошибка при открытии файла
        return false; // сообщаем об этом

    if (layoutDoc.GetRoot() == nullptr) // если не валидный XML
        return false; // возвращаем ошибку

    return true; // всё хорошо
}

Dimensions XmlLayout::getDimensions() {
    auto root = layoutDoc.GetRoot(); // сохраняем в ссылке указатель на корневой элемент XML-документа

    lx = wxAtoi(root->GetAttribute("lx")); // считываем минимальный x
    ly = wxAtoi(root->GetAttribute("ly")); // считываем минимальный y

    return {wxAtoi(root->GetAttribute("layers")), // считываем глубину карты
            wxAtoi(root->GetAttribute("ux")) + 2 - lx, // считываем ширину карты - минимальный x
            wxAtoi(root->GetAttribute("uy")) + 2 - ly}; // считываем высоту карты - минимальный y
}

void XmlLayout::readLayout(TLVec& table) {
    wxXmlNode* tilePtr = layoutDoc.GetRoot()->GetChildren(); // получаем первый дочерний элемент корневого XML-элемента

    int x, y, l;

    while (tilePtr) {
        if (tilePtr->GetName().IsSameAs("tile")) { // если это - XML-нода, описывающая позицию камня
            x = wxAtoi(tilePtr->GetAttribute("x")) - lx; // считываем позицию по x и сдвигаем влево на минимальную позицию (чтобы всегла самый левый камень был в 0)
            y = wxAtoi(tilePtr->GetAttribute("y")) - ly; // считываем позицию по y и сдвигаем вверх на минимальную позицию (чтобы всегла самый верхний камень был в 0)
            l = wxAtoi(tilePtr->GetAttribute("layer")) - 1; // считываем координату по оси z (начиная с 0)

            table[l][x][y] = FREE; // указываем в таблице, что в эту позицию можно поставить камень
        }

        tilePtr = tilePtr->GetNext(); // получаем новый элемент из связанного списка дочерних элементов рут-XML-ноды
    }
}

uint8_t XmlLayout::getTilesNumber() {
    return wxAtoi(layoutDoc.GetRoot()->GetAttribute("tiles")); // получаем общее количество используемых id (на случай, если программе подсунули карту не с 144 камнями)
}