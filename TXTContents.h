#ifndef TXTCONTENTS_H
#define TXTCONTENTS_H

#include "wxw.h"

namespace TXTContents {
    // clang-format off
    const wxString help(_(
        "  При запуске программы вы получили запрос на выбор файла с картой. В случае, если вы его выбрали, перед вами должно быть игровое поле и вы можете приступать к игре, при необходимости ознакомившись с правилами (F1, или в меню \"Помощь->Правила игры\").\n"
        "  Для выбора камня, необходимо нажать на него левой кнопкой мыши. Если он доступен для удаления, он подсветится зелёным цветом. При выборе второго такого же они оба исчезают. \n При клике на другой доступный камень (не считающийся с ним одинаковым), выделение с предыдущего снимается и выделяется другой. \n"
        "  При желании вы можете изменить размеры окна. При этом все элементы окна и игровое поле подстроятся под новый. \n"
        "  Для того, чтобы начать игру сначала, можно нажать в меню \"Игра->Начать сначала\" (или Ctrl+N). \n"
        "  Если вы хотите сменить карту, выберите в меню \"Игра->Открыть карту\" (или Ctrl+O). \n"
        "  Приложение поддерживает два режима генерации карты: полностью случайный и имеющий хотя бы один способ решения. Для выбора, в каком режиме генерировать, в меню есть флажок \"Игра->Генерировать решаемую карту\". \n"
        "  Если в процессе игры вам понадобилось отменить ход, или у вас не осталось ходов, вы можете воспользоваться кнопками \"Игра->Отменить ход\" и \"Игра->Перемешать\" соответственно. \n"
        "  Для выхода из игры, нажмите \"Игра->Выход\" (или Ctrl+Q) \n"
        "  Во вкладке \"Помощь\" так же расположены сведения \"О программе\", где приведены список использованных ресурсов и лицензии. \n"
        "  В нижней части экрана расположена строка состояния. В ней слева выводится время, прошедшее с начала игры, или подскаски по пунктам меню при наведении на них мыши. Справа выводится процент оставшихся камней, которые необходимо убрать, то есть, в начале игры это 100%, когда останется половина камней, будет 50%, а когда будут убраны все, 0%."
    ));
    // clang-format on

    // clang-format off
    const wxString rules(_(
        "  Игра Маджонг – настольная игра для одного человека, напоминающая карточный пасьянс. В ней используется набор из 144 костей таких же как в одноимённой азартной игре. Они раскладываются на поле в случайном порядке, образуя многослойную фигуру. \n"
        "  В игре используются фишки трёх видов: масти, козыри и цветы. Существуют три масти – медяки, бамбуки и тьмы, пронумерованные от одного до девяти, по четыре комплекта каждой. Козыри подразделяются на ветры и драконов. Есть четыре ветра – Восточный, Южный, Западный и Северный по 4 шт. каждого вида. И три дракона – Красный, Зелёный и Белый по 4 шт. каждого вида. Цветы делятся на 4 цветка –  слива, орхидея, хризантема и бамбук и 4 времени года – весна, лето, зима и осень, каждых по 1 шт. \n"
        "  Цель игры состоит в том, чтобы, убирая пары одинаковых не заблокированных фишек, очистить поле. Заблокированными считаются фишки, накрытые другими, или не имеющие свободной правой, или левой стороны. \n"
        "  Одинаковыми считаются медяки, козыри и цветы одной масти и достоинства, одинаковые драконы и ветра. Все четыре кости-цветка и времени года считаются одинаковыми."
    ));
    // clang-format on

    // clang-format off
    const wxString about(_(
        "Приложение wxMahjong (Solitare)     \n"
        "Разработчик: Шишков Дмитрий Андреевич (dm1sh) \n"
        "Группа: 1181 \n"
        "Проверяющий: Ситников И.Ю. \n"
        "\n"
        "  Приложение разработано с использованием кроссплатформенной библиотеки wxWidgets. Она использовалась для создания интерфейса программы и работы с файлами схем карт.\n"
        "  Приложение поддерживает схемы карт в формате .smlf (подробнее можно прочитать здесь https://star.physics.yale.edu/~ullrich/software/SolitaireMahjong/layouts.html). В состав распространяемого набора файлов входят схемы, взятые из программы Томаса Уллрича (https://star.physics.yale.edu/~ullrich/), распространяемы по лицении GNU GPL. \n"
        "  В качестве иконок используется набор, нарисованный FluffyStuff (https://github.com/FluffyStuff/riichi-mahjong-tiles), лицензированные по CC BY 4.0 \n"
        "\n"
        "  Лицензии программы и используемых сторонних ресурсов: \n"
        "\n"
        "wxMahjong (Solitare):\n"
        "\n"
        "Copyright 2022 dm1sh\n"
        "\n"
        "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n"
        "\n"
        "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n"
        "\n"
        "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n"
        "\n"
        "\n"
        "Схемы smlf:\n"
        "\n"
        "Solitaire Mahjong 2.2\n"
        "\n"
        "Copyright (C) 2005-2017 Thomas S. Ullrich\n"
        "\n"
        "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.\n"
        "\n"
        "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n"
        "\n"
        "You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
        "\n"
        "Картинок для камней:\n"
        "\n"
        "This work is licensed under the Creative Commons Attribution 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.\n"
    ));
    // clang-format on
}

#endif
