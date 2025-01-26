# Battleships

The iconic battleships game. Now in your console!

![screenshots of different game screens](https://github.com/user-attachments/assets/250df159-fd04-46a3-b8a6-819c461a24be)

## 🚢 Table of Contents

- [Battleships](#battleships)
    - [🚢 Table of Contents](#-table-of-contents)
    - [🐠 Features](#-features)
    - [🐬 Core Technology Highlights](#-core-technology-highlights)
    - [🏝️ System Requirements](#-system-requirements)

## 🐠 Features

## 🐬 Core Technology Highlights

This project was built with the goal of not using any external libraries, relying entirely on the features provided by
C++20 and UNIX sockets. The game includes a user interface and network serialization, all implemented from
scratch.

- Elegant and colorful user interface built
  using [ANSI escape sequences](https://en.wikipedia.org/wiki/ANSI_escape_code)
- Separate threads for keyboard input and network operations
- Low level networking with UNIX sockets
- Smart memory management
- Modern C++20 features, e.g. `std::ranges`

## 🏝️ System Requirements

The game was written and tested on macOS Sequoia 15.3. It should also work on other UNIX-based systems like Linux.

- A **macOS** computer (or other UNIX-based system).
- A terminal supporting **ANSI escape sequences** (e.g. Terminal.app).
- A terminal window of size **80x25 characters** (width x height).
