#include "../inc/trep.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void ajoute_random_champ(int index) {
    int type = rand() % 4;
    int num_lex = rand() % TLEX_TAILLE_MAX;
    trep_ajoute_structure_champ(index, type, num_lex, 0);
}

static void ajoute_random_dimension(int index) {
    int borne_inf = rand() % 100 - 100;
    int borne_sup = rand() % 100 + 100;
    trep_ajoute_tableau_dimension(index, borne_inf, borne_sup);
}

static void ajoute_random_parametre(int index) {
    int type = rand() % 4;
    int num_lex = rand() % TLEX_TAILLE_MAX;
    trep_ajoute_fonction_param(index, num_lex, type);
}

static void update_random_entree(int index) {
    int i;
    int nb_champs = rand() % 5;
    int nb_dimensions = rand() % 3 + 1;
    int nb_parametres = rand() % 8;
    switch (trep[index].nature) {
        case NATURE_STRUCTURE:
            for (i = 0; i < nb_champs; i++) {
                ajoute_random_champ(index);
            }
            break;
        case NATURE_TABLEAU:
            for (i = 0; i < nb_dimensions; i++) {
                ajoute_random_dimension(index);
            }
            break;
        case NATURE_PROCEDURE:
        case NATURE_FONCTION:
            for (i = 0; i < nb_parametres; i++) {
                ajoute_random_parametre(index);
            }
            break;
        default:
            printf("Erreur - Randomisation de structure inconnue impossible.\n");
            break;
    }
}

static void ajoute_random_entree() {
    int nature = rand() % 4 + 1;
    trep[taille_trep].nature = rand() % 4 + 1;
    if (nature > 2) {
        nature += 2;
    }
    int index = trep_nouvelle_entree(nature);
    update_random_entree(index);
}

int main(void) {
    int i;

    srand(time(NULL));

    init_trep();

    for (i = 0; i < TREP_TAILLE_MAX; i++) {
        ajoute_random_entree();
    }

    printf("Affichage du tableau des representations:\n");
    trep_affiche();
    printf("\n");

    trep_free();
    return EXIT_SUCCESS;
}