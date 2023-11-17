#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "listes.h"
#include "blocs.h"

void free_linked_char_list(sequence_t *seq){
  cellule_t *temp0 = seq->tete;
  free(seq);//frees the head of the linked list
  if (temp0 == NULL) {
    printf("Deleted empty linked list");
    return;} //Case where linked list is empty, we have already freed the head
    printf("The linked list isn't empty :)");
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

/*
blocerr empile_bloc(sequence_d *pile, cellule_t *p_cell){
  //adds a linked list of characters as a datatag to the start of *pile
  //freeing the first and last '{' '}' character cells, 
  //and making routine's head link to the cell after the final '}'
  sequence_t* newbloc = (sequence_t*) malloc(sizeof(sequence_t));
  cellule_t *temp0 = p_cell;
  cellule_t *temp1 = temp0->suivant;

  //We assume that as p_cell is the cell just read by the interpreter, it's value will be '{'
  if (temp0->command != '{') {return ARGERR;} //Checks that the asuumption is true
  if (temp1->command == '}') {//Case where the bloc of commands is empty.
    newbloc->tete = NULL;
    add_command_cell(pile, newbloc);
    return OK;
  }

  int num_parentheses = 1;
  newbloc->tete = temp1; //start of the linked list to be put into a bloc
  while (temp1->command != '}' || num_parentheses != 0) {
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
*/



blocerr empile_bloc(sequence_d *pile, cellule_t *p_cell){
  //adds a linked list of characters as a datatag to the start of *pile
  //freeing the first and last '{' '}' character cells, 
  //and making routine's head link to the cell after the final '}'
  sequence_t* newbloc = (sequence_t*) malloc(sizeof(sequence_t));
  cellule_t *temp0 = p_cell;
  cellule_t *temp1 = temp0->suivant;

  //We assume that as p_cell is the cell just read by the interpreter, it's value will be '{'
  if (temp0->command != '{') {return ARGERR;} //Checks that the asuumption is true

  if (temp1->command == '}') {//Case where the bloc of commands is empty.
    newbloc->tete = NULL;
  }else{

    int num_parentheses = 1;
    newbloc->tete = temp1; //start of the linked list to be put into a bloc
    while (num_parentheses != 0) {//checks when our initial parenthisis is closed
    //total number of open parentheses = 0 when first parenthesis closes
    
      temp0 = temp1;
      if (temp1 == NULL){return PARENTHESEERR;}//checks nullity before iterating
      //if temp1=NULL then we have reached the end of the routine without closing the initial parenthesis
      temp1 = temp1->suivant;
      if (temp1->command == '{') { num_parentheses++; }//everytime we open a parenthesis we add 1
      if (temp1->command == '}') { num_parentheses--; }//everytime we close a parenthesis we subtract 1
    }//at the end of the loop, temp0 is the last command character and temp1 is the final '}'
  
    temp0->suivant = NULL; //cuts the linked list to be just the command string
  }
  temp0 = temp1->suivant; //gets the cell after the last '}', the start of routine
  free(temp1);//frees the last '}'
  p_cell->suivant = temp0;//sets the next character to be looked at to temp0, 
  //skipping all the intermediate cells, which have just been put into a dataTag in the pile
  add_command_cell(pile, newbloc);
  return OK;
}


blocerr dependent_evaluation(sequence_d *pile, cellule_t *p_cell){
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
  //2 cases: execution of either V or F based on value of n, completely frees the bloc not executed
  if (n->value.payload.entier == 0){//execute V
    free_data_cell(V);
    insert_list(F->value.payload.commandes, p_cell);
    free(F->value.payload.commandes);//frees the head of the linked list pointed to by the cell
    free(F);
  }else{ //execute F
    free_data_cell(F);
    insert_list(V->value.payload.commandes, p_cell);
    free(V->value.payload.commandes);//frees the head of the linked list pointed to by the cell
    free(V);
  }
  free(n);//frees cell containing value used
  return OK;
}

blocerr execute_top(sequence_d *pile, cellule_t *p_cell){
  cellule_d *e = pile->tete;
  pile->tete = e->suivant;
  assert(e != NULL);
  assert(e->value.ty == COMMANDES);
  insert_list(e->value.payload.commandes, p_cell);
  free(e->value.payload.commandes);//frees the head of the linked list pointed to by the cell
  free(e);
  return OK;
}

blocerr exchange_top_elements(sequence_d *pile){
  cellule_d *a = pile->tete;
  assert(a != NULL);
  cellule_d *b = a->suivant;
  assert(b != NULL);
  a->suivant = b->suivant;
  b->suivant = a;
  pile->tete = b;
  return OK;
}

blocerr rotate_elements(sequence_d *pile){
  cellule_d *x = pile->tete;//number of cells to rotate
  assert(x != NULL);
  cellule_d *n = x->suivant;//size of block of cells passed as 3rd argument
  assert(n != NULL);
  assert(x->value.ty ==ENTIER);
  assert(n->value.ty ==ENTIER);
  //both x and n should be integers
  //we will now label elements a1, an-x, an-x+1, an, and an+1 
  //so as to be able to manipulate the order of the list
  int n_minus_x = n->value.payload.entier - x->value.payload.entier;
  assert(n_minus_x >= 0);
  if(n_minus_x == 0){
    pile->tete = n->suivant;
    free(x);
    free(n);
    return OK; 
    }//if n=x then rotating does nothing, we simply free the first 2 cells 
    //and increment the list head
  cellule_d *a_1 = pile->tete;
  cellule_d *a_nminusx = a_1;

  for(int i = 1; i < n_minus_x; i++ ){
    a_nminusx = a_nminusx->suivant; //iterates n-x-1 times to get from a1 to an-x
    assert(a_nminusx != NULL);
  }
  cellule_d *a_nminusxplus1 = a_nminusx->suivant;
  cellule_d *a_n = a_nminusxplus1;
   for(int i = (n_minus_x + 1); i < n->value.payload.entier; i++ ){
    a_n = a_n->suivant; //iterates x-1 times to get from an-x+1 to an
    assert(a_n != NULL);
  }
  //Start rearranging of the string slices 
  a_nminusx->suivant = a_n->suivant;//an-x will be the nth cell and so links to the rest of the list
  a_n->suivant = a_1; //former last element links to a_1
  pile->tete = a_nminusxplus1;//a_n-x+1 becomes the first element in the list

//now we free the cells containing x and n
//head of pile is already pointing at a_nminusx+1
free(x);
free(n);

return OK;
}

blocerr clone_top_cell(sequence_d *pile){
  assert(pile->tete != NULL);
  cellule_d *clone = clone_data_cell(pile->tete);//clones first cell
  clone->suivant = pile->tete;//links clone to first cell
  pile->tete = clone;//sets clone to be new first cell
  //the secondd cell is what was previously the first cell, the cell we just cloned
  return OK;
}

blocerr delete_top_cell(sequence_d *pile){
  assert(pile->tete != NULL);
  cellule_d *to_be_deleted = pile->tete;
  pile->tete = to_be_deleted->suivant;//skips the cell to be ignored
  //the secondd cell is what was previously the first cell
  free_data_cell(to_be_deleted);//frees the cell to be ignored
  return OK;
}

