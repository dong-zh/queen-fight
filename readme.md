# Deltarune Giga-Queen Punch-Out Recreation

## About

This project is a basic recreation of the Giga-Queen boss fight in Deltarune. It's an assignment for UNSW's graphics course in 21T3.

`assignment_spec.md` contains the original specifications of the assignment.

`assignment_justification.md` contains documentation for the assignment.

## Game Controls

- Arrow up to punch
- Arrow left and right to doge

## Building

You need `cmake` to build the project. For Windows, the graphics course recommended using the `MSYS2` toolchain and `gcc-10`.

In a `bash` shell, run

```bash
./fetch_deps.sh -G Ninja
```

This will fetch all the dependencies. `-G Ninja` is only required in Windows.

If you're using VS Code with the `cmake` extension, you can just press the run button at the bottom of the window. Otherwise,

```bash
cmake build
```

will generate the resource files, and

```bash
cmake --build build
```

will generate the executable in the `bin` folder.

## Credits

[Deltarune](https://deltarune.com/) is a game by [Toby Fox](https://twitter.com/Tobyfox). The sprites were taken from the game and I downloaded them [here](https://www.spriters-resource.com/pc_computer/deltarune/). The sprites were created by Deltarune's main artist [Temmie Chang](https://twitter.com/tuyoki), and her team. The idea for this project was obviously inspired by Deltarune.

The [`chicken3421` library](https://gitlab.cse.unsw.edu.au/COMP3421/chicken3421-package), the `cmake` build environment, `shapes.cpp`, and `shapes.hpp` was set up and written by UNSW's COMP3421 teaching team in 21T3.

This project also uses the [GLFW library](https://www.glfw.org/).
