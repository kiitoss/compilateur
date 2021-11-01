#include "inc/trep.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int taille_trep = 0;

/* Affiche une entree de type structure */
static void affiche_structure(union entree e) {
    struct trep_entree_structure_champ *champ = e.structure.champ;
    printf("nb_champs:\t%d (type/num_lex/exec", e.structure.nb_champs);
    while (champ != NULL) {
        printf(", %d/%d/%d", champ->type, champ->num_lex, champ->exec);
        champ = champ->suivant;
    }
    printf(")");
}

/* Affiche une entree de type tableau */
static void affiche_tableau(union entree e) {
    struct trep_entree_tableau_dimension *dimension = e.tableau.dimension;
    printf("nb_dimensions:\t%d (borne_inf/borne_sup", e.tableau.nb_dimensions);
    while (dimension != NULL) {
        printf(", %d/%d", dimension->borne_inf, dimension->borne_sup);
        dimension = dimension->suivant;
    }
    printf(")");
}

/* Affiche une entree de type fonction ou procedure */
static void affiche_fonction(union entree e) {
    struct trep_entree_fonction_param *parametre = e.fonction.parametre;
    printf("nb_parametres:\t%d (num_lex/type", e.fonction.nb_parametres);
    while (parametre != NULL) {
        printf(", %d/%d", parametre->num_lex, parametre->type);
        parametre = parametre->suivant;
    }
    printf(")");
}

/* Initialise une nouvelle entrée dans la table des representations */
static void init_entree(int index) {
    switch (trep[index].nature) {
        case NATURE_STRUCTURE:
            trep[index].data.structure.nb_champs = 0;
            trep[index].data.structure.champ = NULL;
            break;
        case NATURE_TABLEAU:
            trep[index].data.tableau.type = VALEUR_NULL;
            trep[index].data.tableau.nb_dimensions = 0;
            trep[index].data.tableau.dimension = NULL;
            break;
        case NATURE_PROCEDURE:
        case NATURE_FONCTION:
            trep[index].data.fonction.type = VALEUR_NULL;
            trep[index].data.fonction.nb_parametres = 0;
            trep[index].data.fonction.parametre = NULL;
            break;
        default:
            printf("Erreur - Initialisation de structure inconnue.\n");
            affiche_nature_declaration(trep[index].nature);
            break;
    }
}

/* Affiche la table des representations */
void init_trep() {
    srand(time(NULL));
    int i;
    for (i = 0; i < TREP_TAILLE_MAX; i++) {
        trep[i].nature = rand() % 4 + 1;
        if (trep[i].nature > 2) {
            trep[i].nature += 2;
        }
        init_entree(i);
    }
}

/* Affiche la table des representations */
void trep_affiche() {
    int i;
    printf(
        "---------------------------------------------------------------------"
        "\nindice\t|\tnature\t\t|\tdata\n");

    for (i = 0; i < TREP_TAILLE_MAX; i++) {
        printf("%d\t|\t", i);
        affiche_nature_declaration(trep[i].nature);
        if (trep[i].nature == NATURE_TABLEAU) {
            printf("\t");
        }
        printf("\t|\t");
        switch (trep[i].nature) {
            case NATURE_STRUCTURE:
                affiche_structure(trep[i].data);
                break;
            case NATURE_TABLEAU:
                affiche_tableau(trep[i].data);
                break;
            case NATURE_PROCEDURE:
            case NATURE_FONCTION:
                affiche_fonction(trep[i].data);
                break;
            default:
                printf("Erreur ? NULL.");
                break;
        }
        printf("\n");
    }

    printf(
        "---------------------------------------------------------------------"
        "\n");
}