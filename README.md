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

https://github.com/user-attachments/assets/23f17a97-3381-4610-be5d-74350667720b

- Play against computer or other players in your LAN.\
  <img src="https://github.com/user-attachments/assets/78d07387-a716-49f3-b664-3b8a4a94a91c" alt="application demo" width="480" />
- Start or join a new network game.\
  <img src="https://github.com/user-attachments/assets/7f5d53d5-4add-4add-bce6-dd45c44c41a5" alt="application demo" width="480" />
- Place your battleships manually... or use a single button to distribute them randomly.\
  <img src="https://github.com/user-attachments/assets/ba08c685-5bee-4242-91fd-9d6f36d91dc0" alt="application demo" width="520" />
- Play with your friends and see their actions in real time.\
  <img src="https://github.com/user-attachments/assets/f84c8d27-9fcb-45a2-8124-37073389c1ee" alt="application demo" width="520" />\
  <img src="https://github.com/user-attachments/assets/e1929882-0967-4007-aecb-615f7ff0101a" alt="application demo" width="520" />

## 🐬 Core Technology Highlights

This project was built with the goal of not using any external libraries, relying entirely on the features provided by
C++20 and UNIX sockets. The game includes a user interface and network serialization, all implemented from
scratch.

- Elegant and colorful user interface built.
  using [ANSI escape sequences](https://en.wikipedia.org/wiki/ANSI_escape_code).
- Separate threads for keyboard input and network operations.
- Low level networking with UNIX sockets.
- Smart memory management.
- Modern C++20 features, e.g. `std::ranges`.

## 🏝️ System Requirements

The game was written and tested on macOS Sequoia 15.3. It should also work on other UNIX-based systems like Linux.

- A **macOS** computer (or other UNIX-based system).
- A terminal supporting **ANSI escape sequences** (e.g. Terminal.app).
- A terminal window of size **80x25 characters** (width x height).
