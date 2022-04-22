<div align="center">
  <img width="500px" src="images/ghostlab.png">
</div>

# Ghostlab

Ghostlab is an online matchmaking based game where you take upon yourself to become the best ghost hunter!

## Screenshots

<div align="center">
    <img src="images/screenshot_1.png" height="180px">
    <img src="images/screenshot_2.png" height="180px">
    <img src="images/screenshot_3.png" height="180px">
</div>

## Features

- Chase ghosts through perfect mazes.
- Interactive console.
- Advanced graphical user interface.
- Terminal user interface for maximum compatibility.
- Robust network architecture.
- Works on Linux (Ubuntu/Debian), macOS and Windows (WSL).

## How To Use

### Compile And Launch (C Sources)

The C sources uses CMake as a cross-platform build system, be sure to have it installed.

#### Install Dependencies

##### Linux (Ubuntu/Debian)

- Run `sudo apt-get install libncurses5-dev libsdl2-dev`.

##### macOS

- Run `brew install sdl2`.

#### Compile

- Open a terminal in the project's root directory.
- Run `mkdir ./c/build && cd ./c/build && cmake ./.. && make && cd ./../..` to generate the build configuration and compile.

#### Launch Server

- Run `./c/build/server_gui` to launch the server in GUI mode.
- Alternatively, run `./c/build/server_tui` to launch th server in TUI mode.

#### Launch Client

- Run `./c/build/client_gui` to launch the client in GUI mode.
- Alternatively, run `./c/build/client_tui` to launch the client in TUI mode.

You should always run the server before the client.  
The recommended user interface to launch with is the GUI mode.

### Compile And Launch (Java Sources)

The Java sources uses Gradle as a cross-platform build system, it should install itself automatically when compiling the project.

#### Compile

- Open a terminal in the project's root directory.
- Run `./java/gradlew build` to compile the code.

#### Launch Client

- Run `./java/gradlew run` to run the Java version of the client.

## Dependencies

### C Sources

- [ImGui](https://github.com/ocornut/imgui)
- [cimgui](https://github.com/cimgui/cimgui)
- [ImTui](https://github.com/ggerganov/imtui)
- [cimtui](https://github.com/hugokindel/school-uni-pr6/tree/main/c/deps/cimtui) (made especially for this project)
- [ncurses](https://invisible-island.net/ncurses)
- [SDL](https://www.libsdl.org/index.php)
- [OpenGL](https://www.opengl.org)

### Java Sources

- [ImGui](https://github.com/ocornut/imgui)
- [imgui-java](https://github.com/SpaiR/imgui-java)
- [LWJGL](https://www.glfw.org/)
- [GLFW](https://www.lwjgl.org/)
- [OpenGL](https://www.opengl.org/)

## Contributors

- [LE CORRE Léo](https://github.com/KarmaEssence)
- [JAUROYON Maxime](https://github.com/Maxime-Jauroyon)
- [KINDEL Hugo](https://github.com/hugokindel)

## License

This project is made for educational purposes only and any part of it can be used freely.
