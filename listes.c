#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;

void conversion (char *texte, sequence_t *seq)
{
    cellule_t *last = NULL; // Pointeur vers la dernière cellule créée

    // Vérification du cas où la chaîne est vide
    if (texte[0] == '\0') {
        seq->tete = NULL;
        return;
    }

    // Creating first cellule
    cellule_t *first = (cellule_t *) malloc(sizeof(cellule_t));
    first->command = texte[0];
    first->suivant = NULL;
    last = first;

    // Going through texte by linking the new cells together
    for (int i = 1; texte[i] != '\0'; i++) {
        if (texte[i] == ' ' || texte[i] == '\n') {continue;}
        cellule_t *temp = (cellule_t *) malloc(sizeof(cellule_t));
        temp->command = texte[i];
        temp->suivant = NULL;
        last->suivant = temp;
        last = temp;
    }

    // assigning linked list to our sequence
    seq->tete = first;
}


dataTag *tag_int(int x){
    dataTag *newCom = (dataTag *) malloc(sizeof(dataTag));
    newCom->ty = ENTIER;
    newCom->payload.entier = x;

    return newCom;
}

dataTag *tag_bloc(sequence_t* l_commands){
    dataTag *newCom = (dataTag *) malloc(sizeof(dataTag));
    newCom->ty = COMMANDES;
    newCom->payload.commandes = l_commands;

    return newCom;
}

void add_cell(sequence_d* seq, dataTag *newTag){
    cellule_d *newCell = (cellule_d *) malloc(sizeof(cellule_d));
    newCell->suivant = seq->tete;
    newCell->value = *newTag;
    seq->tete = newCell;
}

void add_int_cell(sequence_d* seq, int x){
    dataTag *d = tag_int(x);
    add_cell(seq, d);
    free(d);
}

void add_command_cell(sequence_d* seq, sequence_t* l_commands){
    dataTag *d = tag_bloc(l_commands);
    add_cell(seq, d);
    free(d);
}

int get_top_int(sequence_d *seq){
    //Returns the integer value of the first element (top) of the pile
    //and removes it from the pile, freeing the cell containing that value
    //Assumptions: the pile is non empty and the first cell contains an integer
    if (seq->tete == NULL){
        printf("La sequence est vide!");
        exit(1);
    }//checks non-emptiness
    if (seq->tete->value.ty != ENTIER){
        printf("Le premier element n'est pas un entier!");
        exit(1);
    }//checks that first element is an int
    cellule_d *top = seq->tete;
    int ret = top->value.payload.entier;
    seq->tete = top->suivant;
    free(top);
    return ret;
}

void insert_list(sequence_t* bloc, cellule_t* routine){
//Adds a list of commands to the start of routine (to be executed immediately)
//Note : DOES NOT FREE THE HEAD OF THE BLOC
    cellule_t *last = bloc->tete;

    if(last == NULL){
        return;
    }

    for( ; last->suivant != NULL; last = last->suivant){}//last->suivant = NULL

    last->suivant = routine->suivant;
    routine->suivant = bloc->tete;
}

Type get_type(cellule_d *cell){
    return cell->value.ty;
}

void afficher_pile(sequence_t *seq){
        printf("Tete->");
    for (cellule_t *curr = seq->tete; curr != NULL; curr = curr->suivant){
        printf("%c->",curr->command);
    }
    printf("NULL");
}

void afficher_d(cellule_d *cell){
    if (cell == NULL){
        printf("NULL");
        return;
    }
    if (cell->value.ty == ENTIER) {
        printf("[Type : Entier; ");
        printf("Valeur : %d", cell->value.payload.entier);
    }else{
        printf("[Type : Liste; ");
        printf("Valeur:");
        afficher_pile(cell->value.payload.commandes);
    }
    printf("]");
}

void afficher_pile_d(sequence_d *seq){
    printf("Tete_D->");
    for(cellule_d *curr = seq->tete; curr != NULL; curr = curr->suivant){
        afficher_d(curr);
        printf("->");
    }
    printf("NULL\n");
}



sequence_t *clone_linked_string(sequence_t *seq){
    sequence_t *clone = (sequence_t *) malloc(sizeof(sequence_t));

    if (seq->tete == NULL){
        clone->tete = NULL;
        return clone;
    }//case where original list is empty, cloning it is just returning another empty list;

    cellule_t *clonecell0 = (cellule_t *) malloc(sizeof(cellule_t));
    cellule_t *clonecell1 = NULL;
    //creates 2 allocations of cells, 
    //we need to 2 beacasue at each iteration we need a reference to the next memeory allocation
    //before we can get rid of the reference to the current cell in order to define suivant
    clonecell0->command = seq->tete->command;
    clone->tete = clonecell0;//makes head of list point to the first cell

    for(cellule_t *original = seq->tete->suivant; original != NULL; original = original->suivant){
        clonecell1 = (cellule_t *) malloc(sizeof(cellule_t));
        clonecell1->command = original->command;//creates clonecell1 as a clone of original
        clonecell0->suivant = clonecell1; //links clonecell1 into the linked list 
        clonecell0 = clonecell1; //iterates so cell0 is upt to date
    }
    clonecell0->suivant = NULL;//ends the cloned linked list 
    return clone; //returns a pointer to the cloned linked list
}

cellule_d *clone_data_cell(cellule_d *cell){
    assert(cell != NULL);

    cellule_d *clone = (cellule_d *) malloc(sizeof(cellule_d));//allocates memeory to the mew cell
    clone->suivant = NULL;//the following cell will always be null
    //linking the cell to the rest of the list happens outside the scope of this function

    if(cell->value.ty == ENTIER){
        clone->value.ty = ENTIER;
        clone->value.payload.entier = cell->value.payload.entier;
        return clone;
    }//Case where cell contains an integer

    //Case where cell contains a linked string:
    //clones the linked string inside the cell
    sequence_t *newstring = clone_linked_string(cell->value.payload.commandes);
    clone->value.ty = COMMANDES;
    clone->value.payload.commandes = newstring;
    return clone;
}
