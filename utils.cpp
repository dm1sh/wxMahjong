#include "utils.h"

wxString LTimeToStr(int time) {
    return wxString::Format(_("%d:%02d:%02d"), time / 3600, (time / 60) % 60,
                            time % 60); // переводим количество секунд с начала игры в читабельное время (ч:мм:сс) 
}

wxString itowxS(int a) {
    return wxString::Format("%i", a);
}

wxString PRemaining(uint8_t remaining) {
    return wxString::Format("%i%%", remaining * 100 / 144); // делим количество оставшихся камней на их количество и домножаем на 100, чтобы получить проценты
}

bool isPositive(const wxSize& size) {
    return size.x > 0 && size.y > 0;
}

