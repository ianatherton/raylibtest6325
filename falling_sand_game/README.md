# Falling Sand Game (C + Raylib)

A simple 2D falling sand game made with C and the Raylib library.

## Prerequisites

*   A C compiler (e.g., GCC)
*   Make
*   Raylib library installed. You can find installation instructions on the [Raylib website](https://www.raylib.com/).
    *   Ensure Raylib headers and libraries are in your compiler's search paths. The Makefile assumes `/usr/local/include` and `/usr/local/lib`. You might need to adjust these paths in the `Makefile` (CFLAGS and LDFLAGS) if Raylib is installed elsewhere.

## How to Build

1.  Navigate to the project root directory (`falling_sand_game`).
2.  Run the `make` command:

    ```bash
    make
    ```

    This will compile the source code and create an executable named `falling_sand` in the project root.

## How to Run

After building, you can run the game with:

```bash
./falling_sand
```

Or, you can use the make target:

```bash
make run
```

## How to Clean

To remove the compiled object files and the executable:

```bash
make clean
```
