# `⣼⢠⡄⢤⠄⡤⢠⢀⡤`

Mini Tetris made with dots

![gameplay](./dotris.gif)

- [How To Play](#how-to-play)
    - [Controls](#controls)
    - [Terminal Zoom](#terminal-zoom)
- [Mechanics](#mechanics)
    - [Scoring](#scoring)
    - [Levels](#levels)
- [Troubleshooting](#troubleshooting)
- [Installing](#installing)
- [Building](#building)
    - [GCC](#gcc)
    - [Meson](#meson)
    - [GNU Make](#gnu-make)

## How To Play

### Controls

Key           | Description
---           | ---
`Left Arrow`  | Move left
`Right Arrow` | Move right
`Up Arrow`    | Rotate clockwise
`Down Arrow`  | Soft drop (move down)
`Spacebar`    | Hard drop
`C`           | Hold Tetrimino / Restore Held
`Q`           | Quit game

### Terminal Zoom

It is helpful if you increase the font size in your terminal emulator for a "regular"
sized game.

## Mechanics

### Scoring

The scoring is based on the BPS version of Tetris:

Lines      | Points
---        | ---
1          | 40
2          | 100
3          | 300
4 (tetris) | 1200

If the piece used to clear was hard dropped you get 1 point per row added to the point amount.

Each level acts as a multiplier to the above table; i.e on level two, clearing one line is 80.

### Levels

Every 10 lines cleared the speed of the falling piece increases by 40% and the grid is wiped.

## Troubleshooting

If the pieces are not rendering correctly (weird spacing between dots), you will probably
need to install another font that has 
[Braille pattern](https://en.wikipedia.org/wiki/Braille_Patterns) characters.

Some fonts that have perfect characters for Tetris pieces: 
[Terminus](https://files.ax86.net/terminus-ttf/), 
[Cozette](https://github.com/slavfox/Cozette), 
[Spleen](https://www.cambus.net/spleen-monospaced-bitmap-fonts/)

If you use Kitty terminal emulator (recommended), these characters will be
[rendered programmatically](https://github.com/kovidgoyal/kitty/discussions/6152), not
using the configured font.

## Installing

Currently only available through the [AUR](https://aur.archlinux.org/packages/dotris-git).

## Building

```
git clone https://github.com/trevarj/dotris && cd dotris
```

### GCC
```
gcc -lncursesw -Iinclude/ src/main.c src/tetriminos.c src/grid.c -o dotris
```

### Meson
```
meson setup ./build . --buildtype=release
cd build
meson compile

# (optional)
sudo meson install
```

### GNU Make
```
make
# or
# sudo make install
```

Now you can run the `dotris` binary

```
./dotris
```

