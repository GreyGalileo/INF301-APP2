#include <stdio.h>
#include <stdlib.h>
#include "listes.h"
#include "blocs.h"

void free_linked_char_list(sequence_t *seq){
  cellule_t *temp0 = seq->tete;
  free(seq);//frees the head of the linked list
  if (temp0 == NULL) {return;} //Case where linked list is empty, we have already freed the head
  cellule_t *temp1 = temp0->suivant;
  while (temp1 != NULL){
    free(temp0);
    temp0 = temp1;
    temp1 = temp1->suivant;
  //moves both pointers up the list then frees the earlier one
  }
  free(temp0); //frees the last cell, where temp0->suivant = NULL
}

void free_data_cell(cellule_d *cell){
  if(cell->value.ty == ENTIER){
    free(cell);
    return;
  }

  //dataTag type = COMMANEDS
  free_linked_char_list(cell->value.payload.commandes);
  free(cell);//frees the cell itself
}

void free_linked_data_list(sequence_d *seq){
  cellule_d *temp0 = seq->tete;
  free(seq);//frees the head of the linked list
  if (temp0 == NULL) {return;} 
  //Case where linked list is empty, we have already freed the head
  //so we can simply return.

  cellule_d *temp1 = temp0->suivant;
  while (temp1 != NULL){
    free_data_cell(temp0);
    temp0 = temp1;
    temp1 = temp1->suivant;
  //moves both pointers up the list then frees the earlier one
  //temp1 = temp0->suivant at each iteration of the loop
  }
  free_data_cell(temp0); //frees the last cell, where temp0->suivant = NULL
}


blocerr empile_bloc(sequence_d *pile, cellule_t *p_cell){
  //adds a linked list of characters as a datatag to the start of *pile
  //freeing the first and last '{' '}' character cells, 
  //and making routine's head link to the cell after the final '}'
  sequence_t* newbloc = (sequence_t*) malloc(sizeof(sequence_t));
  cellule_t *temp0 = p_cell;
  cellule_t *temp1 = temp0->suivant;

  //We assume that as p_cell is the cell just read by the interpreter, it's value will be '{'
  if (temp0->command != '{') {return ARGERR;} //Checks that the asuumption is true
  free(temp0);
  if (temp1->command == '}') {//Case where the bloc of commands is empty.
    newbloc->tete = NULL;
    add_command_cell(pile, newbloc);
    return OK;
  }

  int num_parentheses = 1;
  newbloc->tete = temp1; //start of the linked list to be put into a bloc
  while (temp1->command != '}' && num_parentheses != 0) {
    temp0 = temp1;
    if (temp1 == NULL){ return PARENTHESEERR;}//checks nullity before iterating
    //if temp1=NULL then we have reached the end of the routine without closing the initial parenthesis
    temp1 = temp1->suivant;
    if (temp1->command == '{') { num_parentheses++; }
    if (temp1->command == '}') { num_parentheses--; }
  }//at the end of the loop, temp0 is the last command character and temp1 is the final '}'
  
  temp0->suivant = NULL; //cuts the linked list to be just the command string
  temp0 = temp1->suivant; //gets the cell after the last '}', the start of routine
  free(temp1);//frees the last '}'
  p_cell->suivant = temp0;//sets the next character to be looked at to temp0, 
  //skipping all the intermediate cells, which have just been put into a dataTag in the pile
  add_command_cell(pile, newbloc);
  return OK;
}

blocerr execute_bloc(sequence_d *pile, cellule_t *p_cell){
  cellule_d *F = pile->tete;
  cellule_d *V = F->suivant;
  cellule_d *n = V->suivant;
  //Verifies the types of the arguments
  //Note: If there is a type error, the arguments REMAIN ON THE PILE and AREN'T FREED
  if (get_type(F) != COMMANDES){
    printf("Le premier argument F n'est pas un bloc de commandes");
    return TYPEERR;
  }
  if (get_type(V) != COMMANDES){
    printf("Le deuxieme argument V n'est pas un bloc de commandes");
    return TYPEERR;
  }
  if (get_type(n) != ENTIER){
    printf("Le troisieme argument n n'est pas un entier");
    return TYPEERR;
  }

  pile->tete = n->suivant;
  //Disjonction de cas: execution de V ou bien F
  if (n->value.payload.entier == 0){//execute V
    free_data_cell(F);
    insert_list(V->value.payload.commandes, p_cell);
    free(V);
  }else{ //execute F
    free_data_cell(V);
    insert_list(F->value.payload.commandes, p_cell);
    free(F);
  }
  return OK;
}