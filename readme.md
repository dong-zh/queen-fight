# Deltarune Giga-Queen Punch-Out Recreation

## About

This project is a basic recreation of the Giga-Queen boss fight in Deltarune. It's an assignment for UNSW's graphics course in 21T3.

[`assignment_specs.md`](assignment_specs.md) contains the original specifications of the assignment.

[`assignment_justification.md`](assignment_justification.md) contains documentation for the assignment.

You can download the compiled Windows binary in the [releases](https://github.com/dong-zh/queen-fight/releases) section. You must extract the archive before running it. Alternatively, you can compile it yourself with the [instructions](#building) below.

## Game Controls

- Arrow up to punch
- Arrow left and right to doge

## Building

### Prerequisites

You'll need

- [`cmake`](https://cmake.org/)
- A C++ compiler for [`cmake`](https://cmake.org/) to discover, like [`clang++`](https://clang.llvm.org/)
- [`ninja`](https://ninja-build.org/)
- [Git](https://git-scm.com/)
- [Python 3](https://www.python.org/)

### Steps

1. Clone this repository and `cd` into the directory

1. Generate build files with

    ```sh
    cmake -G Ninja -B build
    ```

1. Build the code with

    ```sh
    ninja -C build
    ```

The executable (`nutdealer`) will be in the `bin` directory. You must `cd` into that directory before running the executable.

## Credits

[Deltarune](https://deltarune.com/) is a game by [Toby Fox](https://twitter.com/Tobyfox). The sprites were taken from the game and I downloaded them [here](https://www.spriters-resource.com/pc_computer/deltarune/). The sprites were created by Deltarune's main artist [Temmie Chang](https://twitter.com/tuyoki), and her team. The idea for this project was obviously inspired by Deltarune.

The [`chicken3421` library](https://github.com/dong-zh/chicken3421), the `cmake` build environment, `shapes.cpp`, and `shapes.hpp` was set up and written by UNSW's COMP3421 teaching team in 21T3. I rewrote the build steps and slightly modified and integrated the `chicken3421` library into this project.

This project also fetches GLFW, GLAD, and GLM from [this](https://github.com/cmmw/imgui-glfw-glad-glm.git) repository, and [`tinyobjloader`](https://github.com/tinyobjloader/tinyobjloader).
