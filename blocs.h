#ifndef BLOCS_H
#define BLOCS_H

#include "listes.h"

typedef enum {OK, TYPEERR, PARENTHESEERR, ARGERR} blocerr;


void free_linked_char_list(sequence_t *);

void free_data_cell(cellule_d *);

void free_linked_data_list(sequence_d *);

blocerr empile_bloc(sequence_d *, cellule_t *);

blocerr dependent_evaluation(sequence_d *, cellule_t *);

blocerr execute_top(sequence_d *, cellule_t *);

blocerr exchange_top_elements(sequence_d *);

blocerr rotate_elements(sequence_d *);

blocerr clone_top_cell(sequence_d *);

blocerr delete_top_cell(sequence_d *);

blocerr loop_once(sequence_d*, cellule_t*);

#endif