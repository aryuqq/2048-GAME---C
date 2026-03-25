#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  //avem nevoie de timp ca de fiecare datacand rulam jocul sa apara numere diferite in pozitii diferite
#include <unistd.h>

#define DIM 4
#define COLOR_PASTEL_GREEN 13
#define COLOR_PINK 13
#define COLOR_VERDE 14
#define COLOR_PASTEL_BLUE 15
#define COLOR_PASTEL_RED 16
#define COLOR_NEON_RED 17

struct celula {
  int nr;
};

struct celula tabla[DIM][DIM];

void salvare_in_fisier();

void joc_nou(struct celula tabla[DIM][DIM]) {
  int coloana, linie;
  salvare_in_fisier(tabla);
  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = 0; linie < DIM; linie++) {
      tabla[linie][coloana].nr = 0;
    }
  }
}

// cream o functie care sa ne genereze numerul aleatorii pe tabla ( 2 sau 4)
// aceasta va fi folosita pentru jocul nou ca sa ne adauge 2 numere
void numere_aleatorii(struct celula tabla[DIM][DIM]) {
  int linie_aleatorie = 0;
  int coloana_aleatorie = 0;
  int numarul;
  int i;
  for (i = 0; i < 2; i++) {
    // vom folosi rand si pentru linii si coloane ca sa apara aleatoriu pe tabla
    linie_aleatorie = rand() % DIM;
    coloana_aleatorie = rand() % DIM;
    // daca vom avea numar par va iesi 2 iar daca este numar impar va iesi 4
    numarul = (rand() % 2 + 1) * 2;
    tabla[linie_aleatorie][coloana_aleatorie].nr = numarul;
  }
}

int numarare_zerouri(struct celula tabla[DIM][DIM]);

// functia aceasta o folosim pentru a adauga cate un numer aleatoriu cand facem
// o miscare
void adauga_numar(struct celula tabla[DIM][DIM]) {
  int linie_aleatorie, coloana_aleatorie;
  // verificam data tabla noasta mai are sau nu loc
  if (numarare_zerouri(tabla) == 0) {
    return;
  }
  // daca avem spatiu vom adauga un numar pe o pozitie aleatorie
  do {
    linie_aleatorie = rand() % DIM;
    coloana_aleatorie = rand() % DIM;
  } while (tabla[linie_aleatorie][coloana_aleatorie].nr != 0);

  int numar = (rand() % 2 + 1) * 2;
  tabla[linie_aleatorie][coloana_aleatorie].nr = numar;
}

// functa aceasta verifica daca jocul este sau nu terminat
int game_over(struct celula tabla[DIM][DIM]) {
  int linie;
  int coloana;
  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = 0; linie < DIM; linie++) {
      // daca nu mai exista celule goale
      if (tabla[linie][coloana].nr != 0) {
        // verificam daca exista vecini comuni care se pot cupla
        // daca exista va da return 0 care inseamna ca jocul nu este incheiat
        if (tabla[linie][coloana].nr == tabla[linie][coloana - 1].nr) {
          return 0;
        }
        if (tabla[linie][coloana].nr == tabla[linie][coloana + 1].nr) {
          return 0;
        }
        if (tabla[linie][coloana].nr == tabla[linie + 1][coloana].nr) {
          return 0;
        }
        if (tabla[linie][coloana].nr == tabla[linie - 1][coloana].nr) {
          return 0;
        }
      } else {
        return 0;
      }
    }
  }
  return 1;  // jocul este terminat
}

// tre sa fac pentru joc castigator

// --------------- DESIGN ---------------

void design(struct celula tabla[DIM][DIM]) {
  int i;
  int j;
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  // verificam daca terminalul poate arata culorile
  if (!has_colors()) {
    endwin();
    printf("Terminalul nu suportă culori.\n");
    return;
  }
  // initiem culorile celulelor
  init_color(COLOR_VERDE, 0, 1000, 0);
  init_color(COLOR_PINK, 1000, 752, 796);
  init_color(COLOR_PASTEL_GREEN, 466, 866, 466);
  init_color(COLOR_PASTEL_BLUE, 678, 847, 901);
  init_color(COLOR_PASTEL_RED, 1000, 498, 498);
  init_color(COLOR_NEON_RED, 678, 847, 901);
  init_pair(1, COLOR_BLACK, COLOR_PASTEL_GREEN);
  init_pair(2, COLOR_WHITE, COLOR_GREEN);
  init_pair(3, COLOR_BLACK, COLOR_PASTEL_BLUE);
  init_pair(4, COLOR_WHITE, COLOR_BLUE);
  init_pair(5, COLOR_WHITE, COLOR_CYAN);
  init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(7, COLOR_WHITE, COLOR_YELLOW);
  init_pair(8, COLOR_BLACK, COLOR_PINK);
  init_pair(9, COLOR_BLACK, COLOR_PASTEL_RED);
  init_pair(10, COLOR_WHITE, COLOR_NEON_RED);
  init_pair(11, COLOR_WHITE, COLOR_RED);

  // facem modelul tablei de joc
  printw(" ");
  for (i = 0; i < DIM; i++) {
    printw("~~~~~");
  }
  printw("\n");

  for (i = 0; i < DIM; i++) {
    printw("||");
    for (j = 0; j < DIM; j++) {
      int numar = tabla[i][j].nr;
      int color_pair = 0;
      // facem sortarea pentru ficare numar si ii atribuim o pereche de culori
      if (numar == 2) {
        color_pair = 1;
      } else if (numar == 4) {
        color_pair = 2;
      } else if (numar == 8) {
        color_pair = 3;
      } else if (numar == 16) {
        color_pair = 4;
      } else if (numar == 32) {
        color_pair = 5;
      } else if (numar == 64) {
        color_pair = 6;
      } else if (numar == 128) {
        color_pair = 7;
      } else if (numar == 257) {
        color_pair = 8;
      } else if (numar == 512) {
        color_pair = 9;
      } else if (numar == 1024) {
        color_pair = 10;
      } else if (numar == 2048) {
        color_pair = 11;
      }

      // daca avem perechi de culori le vom aplica cu functiile respective din
      // ncurses
      if (color_pair != 0) {
        attron(COLOR_PAIR(color_pair));
      }

      printw(" %d ", numar);

      if (color_pair != 0) {
        attroff(COLOR_PAIR(color_pair));
      }

      printw("||");
    }
    printw("\n");
    if (i <= DIM - 1) {
      printw(" ");
      for (j = 1; j <= DIM; j++) {
        printw("~~~~~");
      }
      printw("\n");
    }
  }
  printw(" ");

  // pentru ora si data
  time_t timp_curent;
  time(&timp_curent);

  struct tm* timp_local = localtime(&timp_curent);

  printw("\n");
  printw("Data si ora curenta: %s\n", asctime(timp_local));
  printw("\n");
  // Panoul de comenzi
  printw("Comenzi valide\n");
  printw("\n");
  printw("Pentru mutare stanga : sageata stanga\n");
  printw("Pentru mutare dreapta : sageata dreapta\n");
  printw("Pentru mutare sus : sageata jus \n");
  printw("Pentru mutare jos : sageata jos\n");
  printw("Pentru a te ajuta robotelul gigel apasa 'a'\n");
  printw("Pentru a selecta optiunile din meniu apasa 'ENTER'\n");
  printw("Pentru a te intoarce in meniu apasa 'q'\n");

  endwin();
}

// __________________________ IMPLEMENTARE COD
// __________________________________

// functia aceata afiseaza ce am savat noi in fisier
// este facuta cu scopul de a arata tabla dupa fiscare modificare salvata
void afisare_din_fisier(struct celula tabla[DIM][DIM]) {
  int linie, coloana;
  FILE* fisier;
  fisier = fopen("etapa_actuala.txt", "r");

  for (linie = 0; linie < DIM; linie++) {
    for (coloana = 0; coloana < DIM; coloana++) {
      fscanf(fisier, "%d", &tabla[linie][coloana].nr);
    }
  }

  fclose(fisier);
}

// facem o functie ca sa salvam intr un fisier stagiul tablei noastre
void salvare_in_fisier(struct celula tabla[DIM][DIM]) {
  int linie;
  int coloana;
  FILE* fisier;
  fisier = fopen("etapa_actuala.txt", "w");

  for (linie = 0; linie < DIM; linie++) {
    for (coloana = 0; coloana < DIM; coloana++) {
      fprintf(fisier, "%d ", tabla[linie][coloana].nr);
    }
    fprintf(fisier, "\n");
  }

  fclose(fisier);
}

void mutare_dreapa_spre_ingerasul_cel_bun(struct celula tabla[DIM][DIM],
                                          int* punctaj) {
  int linie, coloana;
  int repetare;
  for (linie = 0; linie < DIM; linie++) {
    for (coloana = DIM - 2; coloana >= 0; coloana--) {
      // verifică dacă celula curentă nu este goală
      if (tabla[linie][coloana].nr != 0) {
        // cautam unde am putea cupla numarul gasit
        for (repetare = coloana + 1; repetare < DIM; repetare++) {
          // daca vedem spatiu, sarim peste el
          if (tabla[linie][repetare].nr == 0) {
            continue;
          }
          // daca gasim acelasi numar il cuplam si acualizam punctajul
          if (tabla[linie][repetare].nr == tabla[linie][coloana].nr) {
            tabla[linie][repetare].nr = tabla[linie][coloana].nr * 2;
            *punctaj += tabla[linie][repetare].nr;
            tabla[linie][coloana].nr = 0;
            break;
          } else {
            break;
          }
        }
      }
    }
  }

  for (linie = 0; linie < DIM; linie++) {
    for (coloana = DIM - 2; coloana >= 0; coloana--) {
      // verifică dacă celula curentă nu este goală
      if (tabla[linie][coloana].nr != 0) {
        // cautam unde am putea cupla numarul gasit
        for (repetare = coloana + 1; repetare < DIM; repetare++) {
          // dacă găsește spațiu și celula de langa este goală, mutăm numărul
          if (tabla[linie][repetare].nr != 0 &&
              tabla[linie][repetare - 1].nr == 0) {
            tabla[linie][repetare - 1].nr = tabla[linie][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          }
          // dacă suntem la sfârșitul liniei și ultima celulă este goală, mutăm
          // numărul acolo
          if (repetare == DIM - 1 && tabla[linie][repetare].nr == 0) {
            tabla[linie][repetare].nr = tabla[linie][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          }
        }
      }
    }
  }
  // actualizam tabla dupa mutarea facuta
  salvare_in_fisier(tabla);
}

// la fel am facut pentru toate celelalte mutari ajustand doar modul de
// deplasare
void mutare_stanga_spre_ingerasul_cel_rau(struct celula tabla[DIM][DIM],
                                          int* punctaj) {
  int linie, coloana, repetare;

  for (linie = 0; linie <= DIM; linie++) {
    for (coloana = 1; coloana <= DIM - 1; coloana++) {
      if (tabla[linie][coloana].nr != 0) {
        // cautam unde am putea cupla numarul gasit
        for (repetare = coloana - 1; repetare >= 0; repetare--) {
          // daca vedem spatiu, sarim peste el
          if (tabla[linie][repetare].nr == 0) {
            continue;
          }
          // daca gasim acelasi numar, il cuplam
          if (tabla[linie][repetare].nr == tabla[linie][coloana].nr) {
            tabla[linie][repetare].nr = tabla[linie][coloana].nr * 2;
            *punctaj += tabla[linie][repetare].nr;
            tabla[linie][coloana].nr = 0;
            break;
          } else {
            break;
          }
        }
      }
    }
  }

  for (linie = 0; linie <= DIM; linie++) {
    for (coloana = 1; coloana <= DIM - 1; coloana++) {
      if (tabla[linie][coloana].nr != 0) {
        // cautam unde am putea cupla numarul gasit
        for (repetare = coloana - 1; repetare >= 0; repetare--) {
          // dacă găsește spațiu și celula de langa este goală, mutăm numărul
          if (tabla[linie][repetare].nr != 0 &&
              tabla[linie][repetare + 1].nr == 0) {
            tabla[linie][repetare + 1].nr = tabla[linie][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          }
          if (repetare == 0 && tabla[linie][repetare].nr == 0) {
            tabla[linie][repetare].nr = tabla[linie][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          }
        }
      }
    }
  }
  salvare_in_fisier(tabla);
}

void mutare_jos_spre_dracu_sa_ma_ia(struct celula tabla[DIM][DIM],
                                    int* punctaj) {
  int linie, coloana, repetare;

  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = DIM - 2; linie >= 0; linie--) {
      if (tabla[linie][coloana].nr != 0) {
        // cautam unde am putea cupla numarul gasit
        for (repetare = linie + 1; repetare < DIM; repetare++) {
          // daca vedem spatiu, sarim peste el
          if (tabla[repetare][coloana].nr == 0) {
            continue;
          }
          // daca gasim acelasi numar, il cuplam
          if (tabla[repetare][coloana].nr == tabla[linie][coloana].nr) {
            tabla[repetare][coloana].nr = tabla[linie][coloana].nr * 2;
            *punctaj += tabla[repetare][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          } else {
            break;
          }
        }
      }
    }
  }

  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = DIM - 2; linie >= 0; linie--) {
      if (tabla[linie][coloana].nr != 0) {
        // cautam unde am putea cupla numarul gasit
        for (repetare = linie + 1; repetare < DIM; repetare++) {
          // dacă găsește spațiu și celula de deasupra este goală, mutăm numărul
          if (tabla[repetare][coloana].nr != 0 &&
              tabla[repetare - 1][coloana].nr == 0) {
            tabla[repetare - 1][coloana].nr = tabla[linie][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          }
          // dacă suntem la începutul coloanei și prima celulă este goală, mutăm
          // numărul acolo
          if (repetare == DIM - 1 && tabla[repetare][coloana].nr == 0) {
            tabla[repetare][coloana].nr = tabla[linie][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          }
        }
      }
    }
  }
  // salvam fisierul dupa mutare
  salvare_in_fisier(tabla);
}

void mutare_sus_spre_doamne_doamne(struct celula tabla[DIM][DIM],
                                   int* punctaj) {
  int linie, coloana, repetare;
  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = 1; linie < DIM; linie++) {
      if (tabla[linie][coloana].nr != 0) {
        // cautam unde am putea cupla numarul gasit
        for (repetare = linie - 1; repetare >= 0; repetare--) {
          // daca vedem spatiu, sarim peste el
          if (tabla[repetare][coloana].nr == 0) {
            continue;
          }
          // daca gasim acelasi numar, il cuplam
          if (tabla[repetare][coloana].nr == tabla[linie][coloana].nr) {
            tabla[repetare][coloana].nr = tabla[linie][coloana].nr * 2;
            *punctaj += tabla[repetare][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          } else {
            break;
          }
        }
      }
    }
  }

  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = 1; linie < DIM; linie++) {
      if (tabla[linie][coloana].nr != 0) {
        // cautam unde am putea cupla numarul gasit mergand in sus
        for (repetare = linie - 1; repetare >= 0; repetare--) {
          // dacă găsește spațiu și celula de dedesubt este goală, mutăm numărul
          if (tabla[repetare][coloana].nr != 0 &&
              tabla[repetare + 1][coloana].nr == 0) {
            tabla[repetare + 1][coloana].nr = tabla[linie][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          }
          // dacă suntem la începutul coloanei și prima celulă este goală, mutăm
          // numărul acolo
          if (repetare == 0 && tabla[repetare][coloana].nr == 0) {
            tabla[repetare][coloana].nr = tabla[linie][coloana].nr;
            tabla[linie][coloana].nr = 0;
            break;
          }
        }
      }
    }
  }

  // salvam in fisier dupa murae
  salvare_in_fisier(tabla);
}

//------------------MUTARE INVALIDA------------------

// facem o functie pentru a copia tabla si copia o folosim ca test pentru a
// vedea care mutare elibereaza cat mai multe celule
void copiere_matrice(struct celula tabla_test[DIM][DIM],
                     struct celula tabla[DIM][DIM]) {
  int i, j;

  for (i = 0; i < DIM; i++) {
    for (j = 0; j < DIM; j++) {
      tabla_test[i][j].nr = tabla[i][j].nr;
    }
  }
}

int mutare_invalida_dreapta(struct celula tabla_test[DIM][DIM],
                            struct celula tabla[DIM][DIM]) {
  int puncte = 0;
  int mutare_invalida = 0;
  int linie, coloana;
  // copiem tabla
  copiere_matrice(tabla_test, tabla);
  // facem mutarea
  mutare_dreapa_spre_ingerasul_cel_bun(tabla_test, &puncte);
  // daca nu se modifica nimic inseamna ca mutarea nu a fost efectuata
  // acest lucru denota faptul ca mutarea este invalida
  // daca fiecare celula din tabla este la fel mutare_valida va fi 16
  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = 0; linie < DIM; linie++) {
      if (tabla_test[coloana][linie].nr == tabla[coloana][linie].nr) {
        mutare_invalida++;
      }
    }
  }

  // daca este 16 inseamna ca mutarea este invalida
  if (mutare_invalida == 16) {
    return 1;  // mutare invalida
  } else {
    return 0;  // mutarea a putut fi facuta
  }
}

// la fel pentru celelalte functii de mutare invalida
int mutare_invalida_stanga(struct celula tabla_test[DIM][DIM],
                           struct celula tabla[DIM][DIM]) {
  int puncte = 0;
  int mutare_invalida = 0;
  int linie, coloana;
  copiere_matrice(tabla_test, tabla);
  mutare_stanga_spre_ingerasul_cel_rau(tabla_test, &puncte);

  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = 0; linie < DIM; linie++) {
      if (tabla_test[coloana][linie].nr == tabla[coloana][linie].nr) {
        mutare_invalida++;
      }
    }
  }

  if (mutare_invalida == 16) {
    return 1;
  } else {
    return 0;
  }
}

int mutare_invalida_jos(struct celula tabla_test[DIM][DIM],
                        struct celula tabla[DIM][DIM]) {
  int puncte = 0;
  int mutare_invalida = 0;
  int linie, coloana;
  copiere_matrice(tabla_test, tabla);
  mutare_jos_spre_dracu_sa_ma_ia(tabla_test, &puncte);

  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = 0; linie < DIM; linie++) {
      if (tabla_test[coloana][linie].nr == tabla[coloana][linie].nr) {
        mutare_invalida++;
      }
    }
  }

  if (mutare_invalida == 16) {
    return 1;
  } else {
    return 0;
  }
}

int mutare_invalida_sus(struct celula tabla_test[DIM][DIM],
                        struct celula tabla[DIM][DIM]) {
  int puncte = 0;
  int mutare_invalida = 0;
  int linie, coloana;
  copiere_matrice(tabla_test, tabla);
  mutare_sus_spre_doamne_doamne(tabla_test, &puncte);

  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = 0; linie < DIM; linie++) {
      if (tabla_test[coloana][linie].nr == tabla[coloana][linie].nr) {
        mutare_invalida++;
      }
    }
  }

  if (mutare_invalida == 16) {
    return 1;
  } else {
    return 0;
  }
}

//--------------- BOOT ---------------

// vom folosi o functie de numarare a zerourilor pentru a vedea care mutare
// produce cele mai multe zerouri

int numarare_zerouri(struct celula tabla[DIM][DIM]) {
  int coloana, linie;
  int numar_zerouri = 0;
  for (coloana = 0; coloana < DIM; coloana++) {
    for (linie = 0; linie < DIM; linie++) {
      if (tabla[coloana][linie].nr == 0) {
        numar_zerouri++;
      }
    }
  }
  return numar_zerouri;
}

// functia va alege care mutare este cea mai eficienta cand vine vorba de nr de
// zerouri
int cautare_mutare_buna(struct celula tabla[DIM][DIM]) {
  int zero_stanga, zero_dreapta, zero_sus, zero_jos;

  struct celula tabla_test[DIM + 1][DIM];
  int puncte = 0;

  // copiere_matrice(tabla_test, tabla);
  // vom copia matricea folosind functia memcpy
  // detaliu nesemnificativ:
  // daca te intrebi de ce am folosit si asa si copiere matrice motivul e unul
  // pozitiv e mult mai eficienta si dupa mi am dat seama
  memcpy(tabla_test, tabla, sizeof(struct celula) * DIM * DIM);
  mutare_stanga_spre_ingerasul_cel_rau(tabla_test, &puncte);
  zero_stanga = numarare_zerouri(tabla_test);

  memcpy(tabla_test, tabla, sizeof(struct celula) * DIM * DIM);
  mutare_dreapa_spre_ingerasul_cel_bun(tabla_test, &puncte);
  zero_dreapta = numarare_zerouri(tabla_test);

  memcpy(tabla_test, tabla, sizeof(struct celula) * DIM * DIM);
  mutare_jos_spre_dracu_sa_ma_ia(tabla_test, &puncte);
  zero_jos = numarare_zerouri(tabla_test);

  memcpy(tabla_test, tabla, sizeof(struct celula) * DIM * DIM);
  mutare_sus_spre_doamne_doamne(tabla_test, &puncte);
  zero_sus = numarare_zerouri(tabla_test);

  int zero_maxim = zero_dreapta;
  int mutarea_buna = 0;
  // vedem care mutare este cea buna
  if (zero_stanga > zero_maxim) {
    zero_maxim = zero_stanga;
    mutarea_buna = 1;
  }

  if (zero_jos > zero_maxim) {
    zero_maxim = zero_jos;
    mutarea_buna = 2;
  }

  if (zero_sus > zero_maxim) {
    zero_maxim = zero_sus;
    mutarea_buna = 3;
  }

  return mutarea_buna;
}

// legenda pentru mutare buna
//  zero pentru dreapta
//  1 pentru stanga
//  2 pentru jos
//  3 pentru sus

// il facem pe robotelul gigi care o sa ne ajute executand muarile eficiente

void robotelul_gigi(struct celula tabla[DIM][DIM], int* punctaj) {
  int mutare_buna;

  mutare_buna = cautare_mutare_buna(tabla);
  // va executa mutarile respective in functie de cea aleasa de dunctia
  // anterioara
  switch (mutare_buna) {
    case 0:
      mutare_dreapa_spre_ingerasul_cel_bun(tabla, punctaj);
      break;
    case 1:
      mutare_stanga_spre_ingerasul_cel_rau(tabla, punctaj);
      break;
    case 2:
      mutare_jos_spre_dracu_sa_ma_ia(tabla, punctaj);
      break;
    case 3:
      mutare_sus_spre_doamne_doamne(tabla, punctaj);
      break;
  }
}

//------------------CONTROALE------------------

// pentru un main mai frumos si scurt am facut functia de controale
// aici vom apela functiile necesare pentru mutari sortate dupa tastele
// corespunzatoare
void controale_joc(struct celula tabla[DIM][DIM],
                   struct celula tabla_test[DIM][DIM]) {
  int punctaj = 0;
  initscr();
  start_color();
  curs_set(0);  // ascunde cursorul
  noecho();
  keypad(stdscr, TRUE);
  int tasta;
  while ((tasta = getch()) != 'q') {
    clear();
    switch (tasta) {
      case KEY_UP:
        mutare_sus_spre_doamne_doamne(tabla, &punctaj);
        adauga_numar(tabla);
        printw("PUNCTAJ: %d\n", punctaj);
        design(tabla);
        int mutare_incorecta_sus;
        mutare_incorecta_sus = mutare_invalida_sus(tabla_test, tabla);
        if (mutare_incorecta_sus == 1) {
          printw("Mutarea in sus este invalida\n");
        }
        break;
        break;
      case KEY_DOWN:
        mutare_jos_spre_dracu_sa_ma_ia(tabla, &punctaj);
        adauga_numar(tabla);
        printw("PUNCTAJ: %d\n", punctaj);
        design(tabla);

        int mutare_incorecta_jos;
        mutare_incorecta_jos = mutare_invalida_jos(tabla_test, tabla);
        if (mutare_incorecta_jos == 1) {
          printw("Mutarea in jos este invalida\n");
        }
        break;
      case KEY_LEFT:
        mutare_stanga_spre_ingerasul_cel_rau(tabla, &punctaj);
        adauga_numar(tabla);
        printw("PUNCTAJ: %d\n", punctaj);
        design(tabla);

        int mutare_incorecta_stanga;
        mutare_incorecta_stanga = mutare_invalida_stanga(tabla_test, tabla);
        if (mutare_incorecta_stanga == 1) {
          printw("Mutarea la stanga este invalida\n");
        }
        break;
      case KEY_RIGHT:
        mutare_dreapa_spre_ingerasul_cel_bun(tabla, &punctaj);
        adauga_numar(tabla);
        printw("PUNCTAJ: %d\n", punctaj);
        design(tabla);

        // struct celula tabla_test[DIM][DIM];

        int mutare_incorecta_dreapta;
        mutare_incorecta_dreapta = mutare_invalida_dreapta(tabla_test, tabla);
        if (mutare_incorecta_dreapta == 1) {
          printw("Mutarea la dreapta este invalida\n");
        }

        break;
      case 97:
        // aici nu m am descurcat cu timpul acela asa ca am ales sa fac cu taste
        // ca sa nu fie degeaba ce am lucrat
        printw(
            "Robotelul gigel este activat.Daca mai vrei ajutorul lui apasa 'a' "
            "de cate ori este nevoie\n ");
        robotelul_gigi(tabla, &punctaj);
        adauga_numar(tabla);
        printw("PUNCTAJ: %d\n", punctaj);
        design(tabla);
        break;
    }
  }
}

// --------------- MAIN ---------------

int main() {
  initscr();
  start_color();
  curs_set(0);  // ascunde cursorul
  noecho();     // nu afișa caracterele introduse de la tastatură
  keypad(stdscr, TRUE);
  if (!has_colors()) {
    printw("NO COLOR");
    return -1;
  }
  srand(time(NULL) * getpid());

  // --------------- MENIU ---------------
  init_pair(10, COLOR_BLACK, COLOR_WHITE);
  int tasta;
  int tasta_meniu;
  int varianta = 0;
  // aici am lucrat repetitiv pentru a motifica lista meniului in functie de
  // pozitia in care te afli
  while ((varianta < 3 && varianta >= 0) || (tasta = getch()) == 'q') {
    clear();
    // am facut variante pentru a reperezenta fiecare optiune din meniu
    // verificam la care optiune ne aflam si afisam meniul ei
    if (varianta == 0) {
      printw("Meniu:\n");
      attron(COLOR_PAIR(10));
      printw("Press enter for NEW GAME\n");
      attroff(COLOR_PAIR(10));
      printw("2) RESUME\n");
      printw("3) QUIT\n");
    } else if (varianta == 1) {
      printw("Meniu:\n");
      printw("1) NEW GAME\n");
      attron(COLOR_PAIR(10));
      printw("Press enter for RESUME\n");
      attroff(COLOR_PAIR(10));
      printw("3) QUIT\n");
    } else if (varianta == 2) {
      printw("Meniu:\n");
      printw("1) NEW GAME\n");
      printw("2) RESUME\n");
      attron(COLOR_PAIR(10));
      printw("Press enter to QUIT\n");
      attroff(COLOR_PAIR(10));
    }

    refresh();

    tasta_meniu = getch();
    // vom folosi taste pentru a naviga in meniu
    switch (tasta_meniu) {
      // daca ne aflam la o varianta din coada folosim tasta sus si in asa fel
      // va scadea varianta si ne va arata celalt meniu
      case KEY_UP:
        if (varianta > 0) {
          varianta--;
          break;
        }
      // daca ne aflam la o varianta de inceput apasam tasta jos si varianta va
      // creste
      case KEY_DOWN:
        if (varianta < 2) {
          varianta++;
          break;
        }
      case 10:
        // daca apasam entar pe varianta 0 va face un joc nou
        if (varianta == 0) {
          clear();
          joc_nou(tabla);
          while (game_over(tabla) == 0) {
            numere_aleatorii(tabla);
            design(tabla);
            printw("\n");
            struct celula tabla_test[DIM][DIM];
            controale_joc(tabla, tabla_test);
          }

        } else if (varianta == 1) {
          // daca vom apasa pe varianta 1 ne va da resume la joc
          afisare_din_fisier(tabla);
          clear();
          design(tabla);
          printw("\n");
          refresh();
          // getch();
          struct celula tabla_test[DIM][DIM];
          controale_joc(tabla, tabla_test);
        } else if (varianta == 2) {
          // daca apasam enter pe varianta 2 ne va scoate din joc
          clear();
          printw("Vrei sa iesi?\n");
          printw("E ok poti apasa orice tasta pentru a iesi din joc\n");
          refresh();
          getch();
          endwin();
          exit(0);
        }
        break;
    }
  }

  // pentru mutare invalida

  endwin();
  return 0;
}
