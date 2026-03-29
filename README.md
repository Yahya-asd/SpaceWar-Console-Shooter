# SpaceWar-Console-Shooter
A C++ arcade space shooter featuring multiple difficulty modes, AABB collision detection, state saving/loading, and high-score tracking via File I/O.



# SpaceWar: C++ Arcade Shooter 🚀

A robust, console-based space shooter game written in C++. This project was built to explore core game development mechanics, including continuous game loops, frame rate normalization, real-time collision detection, and persistent state management using File I/O.

## 🕹️ Features

* **Multiple Game Modes:** Features four distinct modes—Arcade (endless random spawns), Hard (8x10 enemy grid), Medium (6x10 grid), and Easy (4x10 grid).
* **Persistent Game State:** Players can save their current game progress (ship position, bullet arrays, alien grid status) and resume later. Data is serialized and saved to `savegame.txt`.
* **High Score Tracking:** Implements File I/O to read, sort (using Bubble Sort), and write top scores to `highscores.txt`.
* **AABB Collision Detection:** Calculates real-time bounding box collisions between active bullets, the player's ship, and the alien grid.
* **Frame Rate Limiting:** Utilizes `GetTickCount64()` to normalize the game loop to a target 60 FPS, preventing the game from running too fast on modern processors.

## 🛠️ Technical Stack
* **Language:** C++
* **Libraries:** `<iostream>`, `<Windows.h>`, `<fstream>`, `<cstdlib>`, `<ctime>`
* **Graphics/Input:** Custom `help.h` wrapper for Windows API drawing functions.

## 🎮 Controls
* **Up / Down / Left / Right:** Movement (Keys mapped in custom header, typically WASD or Arrow Keys)
* **Fire (Key 5):** Shoot bullets at the alien grid.
* **Save Game (Key 6):** Instantly write your current game state to disk.
* **Pause (Key 7):** Toggle game pause state.
* **Load Game (Key 8):** Read and resume a previously saved game state.

## 💻 Compilation & Execution

**Note:** This project relies on a custom `help.h` header file (often provided in university programming courses) which acts as a wrapper for standard Windows API graphics and input functions (like `myRect`, `drawText`, and `isKeyPressed`). 

To run this locally, ensure you have a C++ compiler set up for Windows (like MinGW or MSVC) and that `help.h` is present in your build directory.

```bash
g++ FINALPROJECT.cpp -o spacewar.exe
./spacewar.exe
