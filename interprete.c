#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"
#include "blocs.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}





int interprete (sequence_t* seq, bool debug)
{
    // Version temporaire a remplacer par une lecture des commandes dans la
    // liste chainee et leur interpretation.

    char commande = '\0';
    
    //debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf ("Programme:");
    afficher_pile(seq);
    printf("\n");
    if (debug) stop();

    // À partir d'ici, beaucoup de choses à modifier dans la suite.
    //printf("\n>>>>>>>>>>> A Faire : interprete.c/interprete() <<<<<<<<<<<<<<<<\n");
    int ret = CIBLERATEE;//utilisée pour les valeurs de retour


    int x,y;//variables pour les operateurs arithmetiques
    sequence_t printable = *seq;

    //Initialisation de la pile:
    sequence_d *pile = (sequence_d *) malloc(sizeof(sequence_d));
    pile->tete = NULL;


    for (cellule_t *curr = seq->tete; curr != NULL; curr=curr->suivant)
    { // à modifier: condition de boucle
        commande = curr->command;

        switch (commande) {
            /* Ici on avance tout le temps, à compléter pour gérer d'autres commandes */

            case 'A':
                ret = avance();
                if (ret == VICTOIRE){
                    free_linked_data_list(pile);
                    return VICTOIRE;
                } /* on a atteint la cible */
                if (ret == RATE){
                    free_linked_data_list(pile);
                    return RATE;
                }     /* tombé dans l'eau ou sur un rocher */
                break; /* à ne jamais oublier !!! */

            case 'D':
                droite();
                break; /* à ne jamais oublier !!! */
            
            case 'G':
                gauche();
                break;

            case 'P':
                x = get_top_int(pile); // Si x = 0, elenve marque sinon pose une marque 
                pose(x);
                if (x == 0){
                    printf("Mark successfully removed\n");
                } else {
                    printf("Mark successfully added\n");
                }
                break;

            case 'M':
                x = get_top_int(pile);//gets int 
                y = mesure(x);//measures
                add_int_cell(pile, y);//adds y to top of stack
                break;

            case '0' ... '9':
                add_int_cell(pile, (int) (commande - '0')); 
                //converts command to its integer form then adds it to the top of the pile
                break;

            case '{':
                empile_bloc(pile, curr);
                break;

            case '?':
                execute_bloc(pile, curr);
                break;

            case '+':
                x = get_top_int(pile);
                y = get_top_int(pile);
                add_int_cell(pile, x+y);
                break;

            case '-':
                x = get_top_int(pile);
                y = get_top_int(pile);
                add_int_cell(pile, y-x);
                break;

            case '*':
                x = get_top_int(pile);
                y = get_top_int(pile);
                add_int_cell(pile, x*y);
                break;
            
            default:
                eprintf("Caractère inconnu: '%c'\n", curr->command);
        }
        if (ret == VICTOIRE || ret == RATE){break;}

        /* Affichage pour faciliter le debug */
        afficherCarte();
        printable.tete = curr;
        printf("Programme:");
        afficher_pile(&printable);
        printf("\n");
        if (debug) {stop();}
        printf("Pile : ");
        afficher_pile_d(pile);
        
    }
    
    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    free_linked_data_list(pile);
    return CIBLERATEE;
}
