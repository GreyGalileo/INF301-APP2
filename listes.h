#ifndef LISTES_H
#define LISTES_H

#include <stdbool.h>

/*
 * Pour réaliser des tests de performance, désactiver tous les 
 * affichages.
 * Pour cela, le plus simple est de redefinir les fonctions principales 
 * en decommentant les 3 lignes suivantes et en commentant l'ancienne 
 * definition de 'eprintf' juste dessous.
 */

#ifdef SILENT

#define printf(fmt, ...) (0)
#define eprintf(fmt, ...) (0)
#define putchar(c) (0)

#else

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif

extern bool silent_mode;

typedef struct cellule{
    char command;
    struct cellule *suivant;
}cellule_t;

typedef struct{
    cellule_t *tete;
}sequence_t;

typedef union{
    int entier;
    sequence_t *commandes;
 }Data;

typedef enum {ENTIER, COMMANDES} Type;

typedef struct{
    Data payload;
    Type ty;
}dataTag;

typedef struct cell{
    dataTag value;
    struct cell *suivant;
}cellule_d;

typedef struct{
    cellule_d *tete;
}sequence_d;


void conversion(char*, sequence_t*);

void add_int_cell(sequence_d*, int);

void add_command_cell(sequence_d*, sequence_t*);

void add_text_Bloc(sequence_d*, sequence_t*);

int get_top_int(sequence_d*);

void insert_list(sequence_t*, cellule_t*);

Type get_type(cellule_d *);

void afficher_pile(sequence_t*);

void afficher_d(cellule_d*);

void afficher_pile_d(sequence_d*);

sequence_t *clone_linked_string(sequence_t *);

cellule_d *clone_data_cell(cellule_d *);

#endif