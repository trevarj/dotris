# `⣼⢠⡄⢤⠄⡤⢠⢀⡤`

Mini Tetris made with dots

![gameplay](./dotris.gif)

## How To Play

Key           | Description
---           | ---
`Left Arrow`  | Move left
`Right Arrow` | Move right
`Up Arrow`    | Rotate clockwise
`Down Arrow`  | Soft drop (move down)
`Spacebar`    | Hard drop
`Q`           | Quit game

## Mechanics

### Scoring

The scoring follows the BPS version of Tetris:

Lines      | Points
---        | ---
1          | 40
2          | 100
3          | 300
4 (tetris) | 1200

If the piece used to clear was hard dropped you get 1 point per row added to the point amount.

### Levels

Every 10 lines cleared the speed of the falling piece increases by 25% and the grid is wiped.

## Building

```
git clone https://github.com/trevarj/dotris && cd dotris
meson setup ./build .
meson configure -Dbuildtype=release
cd build
meson compile
```

Now you can run the `dotris` binary

```
./dotris
```

