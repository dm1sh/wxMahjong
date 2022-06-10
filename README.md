# wxMahjong

<p align="center">
  <img src="./resources/icon.svg" alt="wxMahjong logo" width="150px">
</p>

Mahjong (solitare) game, written with C++ and [wxWidgets](https://www.wxwidgets.org)

## Installation instructions:

Install clang, make and wxWidgets library. e.g:

```bash
sudo pacman -S clang make wxgtk3
```

Build project:

```bash
make
```

Add wxWidgets libs to ld path:

```bash
echo '/usr/local/lib' | tee -a /etc/ld.so.conf
ldconfig
```

Install app:

```bash
make install
```
