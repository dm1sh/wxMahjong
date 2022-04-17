#include "utils.h"

wxString LTimeToStr(int time) {
    return wxString::Format(_("%d:%02d:%02d"), time / 3600, (time / 60) % 60, time % 60);
}

int upDiv(int a, int b) {
    return a / b + ((a % b) ? 1 : 0);
}

wxString itowxS(int a) {
    return wxString::Format("%i", a);
}