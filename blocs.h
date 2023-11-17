#ifndef BLOCS_H
#define BLOCS_H

#include "listes.h"

typedef enum {OK, TYPEERR, PARENTHESEERR, ARGERR} blocerr;


void free_linked_char_list(sequence_t *);

void free_data_cell(cellule_d *);

void free_linked_data_list(sequence_d *);

blocerr empile_bloc(sequence_d *, cellule_t *);

blocerr execute_bloc(sequence_d *, cellule_t *);

blocerr echange_bloc(sequence_d *pile);

blocerr execute_single_bloc(sequence_d *, cellule_t *);

blocerr clone_bloc(sequence_d *pile);

blocerr delete_bloc(sequence_d *pile);

blocerr commande_bloc(sequence_d *pile, cellule_t *p_cell);

#endif
