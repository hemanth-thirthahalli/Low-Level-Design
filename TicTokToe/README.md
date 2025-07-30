
# 🕹️ Tic Tac Toe – Low Level Design (LLD)

## 📌 Overview

A scalable Tic Tac Toe engine that supports:
- ✅ Multiple concurrent games
- ✅ Configurable board sizes
- ✅ Multiple players (2+)
- ✅ Custom win conditions (e.g., 3 in a row, 4 in a row)
- ✅ Modular and testable architecture following SOLID principles

---

## 1. ✅ Functional Requirements

- Create a new game session.
- Configure board size (NxN), number of players, and win condition (e.g., 3 in a row).
- Players take turns placing their symbols.
- Detect winner or draw after each move.
- Show game board after each move.
- Allow multiple games to run independently.

---

## 2. ⚙️ Non-Functional Requirements

- CLI-based for simplicity (pluggable to UI).
- Follows **SOLID principles**.
- Modular, testable, and extendable architecture.
- Efficient and responsive even for large boards.
- Can support AI or remote players in future.

---

## 3. 👥 Actors

| Actor          | Responsibility                                  |
|----------------|--------------------------------------------------|
| **Player**     | Joins a game, makes moves.                       |
| **Game Host**  | Creates game with config (board, players, win).  |
| **GameManager**| Manages all active games.                        |
| **Game Engine**| Drives turn logic, win/draw detection.           |
| **Board**      | Holds the grid state and checks cell validity.   |

---

## 4. 📘 Use Case Diagram

```
     +-----------+                +--------------+
     |  Player   |                |  Game Host   |
     +-----------+                +--------------+
         |                              |
         | Join Game                    | Create Game
         | Make Move                    | Set Rules
         v                              v
               +---------------------------+
               |      GameManager          |
               +---------------------------+
               | Create, Track, End Games  |
               +---------------------------+
                        |
              +------------------+
              |     Game         |
              |------------------|
              | Board, Players,  |
              | Game Logic       |
              +------------------+
```

---

## 5. 🧱 Class Design (SOLID)

### 🔑 Core Classes

- **Player**: Has a name and symbol.
- **Symbol**: Wraps the character used on the board.
- **Board**: NxN grid of characters, provides mark and display.
- **Game**: Contains game loop, win/draw logic, players.
- **GameManager**: Tracks and controls multiple games.
- **Move** (optional): Stores row, column, and player.

### 📐 Class Diagram (Text Form)

```
GameManager
 └── manages → Game
                  ├── uses → Board
                  └── uses → Player (1..n)

Board
 └── has → NxN grid
 └── methods → markCell(), display(), isFull()

Player
 └── fields → name, Symbol

Game
 └── methods → play(), makeMove(), checkWin()
```

### ✅ SOLID Application

| Principle | Application |
|----------|-------------|
| **S**ingle Responsibility | Each class has one responsibility (Game = loop, Board = grid) |
| **O**pen/Closed           | New player types (e.g., AI) can be added without changing logic |
| **L**iskov Substitution   | Subclass of Player can still work with Game |
| **I**nterface Segregation| Can be applied if UI separation is added later |
| **D**ependency Inversion | Game depends on abstractions (Player), not concrete implementations |

---

## 6. 🎞️ Sequence Diagram

### "Create Game & Play"

```
Actor       GameManager    Game           Board         Player
 |               |            |              |              |
 | createGame()  |            |              |              |
 |-------------> |            |              |              |
 |               | create()   |              |              |
 |               |----------> |              |              |
 |               |            | initBoard()  |              |
 |               |            |------------> |              |
 |               |<---------- |              |              |
 |<--------------|            |              |              |
 |               | startGame()|              |              |
 |               |----------> |              |              |
 |               |            | turn(Player) |              |
 |               |            |------------> |              |
 |               |            |              | getMove()    |
 |               |            |              |----------->  |
 |               |            |              | <----------- |
 |               |            | makeMove()   |              |
 |               |            |------------> |              |
 |               |            | checkWin()   |              |
```

---

## 7. 🧪 Future Enhancements

- Add **AI player** (via subclass of `Player`).
- Add **remote play** support.
- Add **REST API** layer or WebSockets for multiplayer.
- Add **logging** and **score tracking**.

---

## 8. 📎 How to Run (CLI Version)

1. Compile with any C++ compiler (e.g., `g++`):
   ```bash
   g++ main.cpp -o tictactoe
   ./tictactoe
   ```

2. Follow CLI instructions to input player moves.
