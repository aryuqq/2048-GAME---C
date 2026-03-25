# 2048-GAME---C

##Description
This project is an implementation of the **2048** game written in C, using the `ncurses` library for a terminal-based interface.
The goal of the game is to combine tiles to reach the value **2048**.
---

##Main Features

###Initialization and Generation

* A new game initializes the board with zeros
* **Two random numbers (2 or 4)** are generated at random positions
* After each valid move, a new number is added to the board

---
###Controls

* `Left Arrow` – move left
* `Right Arrow` – move right
* `Up Arrow` – move up
* `Down Arrow` – move down
* `a` – activate auto-play (robot move)
* `q` – return to menu / quit

---

###Game Logic

* Equal tiles merge (e.g., 2 + 2 → 4)
* Score is calculated based on merges
* The game checks if moves are still possible:

  * empty cells exist
  * or adjacent equal tiles exist

---

###Auto Player (Gigel)

* Evaluates all possible moves
* Chooses the move that results in the **most empty spaces**
* Executes the best move automatically

---

###Save & Resume

* Game state is saved in: `etapa_actuala.txt`
* The **Resume** option loads the last saved state

---

###Interface (ncurses)

* Board rendered in the terminal
* Different colors for each tile value:

  * 2, 4, 8, 16 ... 2048
* Displays:

  * score
  * current date and time
  * available controls

---

##Code Structure

###Data Structures

```c
struct celula {
  int nr;
};
```

---

###Important Functions

#### Initialization

* `joc_nou()` – resets the board
* `numere_aleatorii()` – generates initial numbers
* `adauga_numar()` – adds a new number after a move

#### Movement

* `mutare_dreapta_spre_ingerasul_cel_bun()`
* `mutare_stanga_spre_ingerasul_cel_rau()`
* `mutare_jos_spre_dracu_sa_ma_ia()`
* `mutare_sus_spre_doamne_doamne()`

#### Validation

* `mutare_invalida_*()` – checks if a move changes the board

####BOT

* `cautare_mutare_buna()` – finds the best possible move

---

##How to Run

###Compile

```bash
make
```

###Run

```bash
make run
```

---

##Requirements

* GCC compiler
* `ncurses` library installed

Example (Linux):

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

---

##Notes

* The game runs entirely in the terminal
* Focus is on logic, not graphics
* Code includes both manual play and automated gameplay

---

Enjoy the game!
