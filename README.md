# Quidmasters: Petbuilder

This repository contains the source code for the game *Quidmasters: Petbuilder*.

## Dependencies

- SFML >= 2.6.1
- C++20

## Compiling from source

### Linux/MacOS

Install the respective development SFML package available in your distribution.
In MacOS you can install SFML using `brew install sfml`.

Then you must clone the repository and run make:

```
$ git clone https://github.com/Sinono3/pou
$ cd pou
$ make run
```

The game should compile and run after these steps.

### Windows

You need to install [MSYS2](https://www.msys2.org/) and download the 64-bit UCRT MinGW toolchain and SFML with it.
You can do so by typing this in the MSYS2 UCRT64 terminal:

```
$ pacman -S base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-sfml
```

After this simply run `make run` inside the terminal and inside the project directory and the game should compile and run.

### Windows (Visual Studio)

If you instead want to compile this using Visual Studio, you will have to create a project in Visual Studio and manually configure includes and linking for SFML.
