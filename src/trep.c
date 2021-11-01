#include "inc/trep.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Affiche une entree de type structure */
static void affiche_structure(union entree e) {
    structure_champ *champ = e.structure.champ;
    printf("nb_champs:\t%d (type/num_lex/exec", e.structure.nb_champs);
    while (champ != NULL) {
        printf(", %d/%d/%d", champ->type, champ->num_lex, champ->exec);
        champ = champ->suivant;
    }
    printf(")");
}

/* Affiche une entree de type tableau */
static void affiche_tableau(union entree e) {
    tableau_dimension *dimension = e.tableau.dimension;
    printf("nb_dimensions:\t%d (borne_inf/borne_sup", e.tableau.nb_dimensions);
    while (dimension != NULL) {
        printf(", %d/%d", dimension->borne_inf, dimension->borne_sup);
        dimension = dimension->suivant;
    }
    printf(")");
}

/* Affiche une entree de type fonction ou procedure */
static void affiche_fonction(union entree e) {
    fonction_param *parametre = e.fonction.parametre;
    printf("nb_parametres:\t%d (num_lex/type", e.fonction.nb_parametres);
    while (parametre != NULL) {
        printf(", %d/%d", parametre->num_lex, parametre->type);
        parametre = parametre->suivant;
    }
    printf(")");
}

/* Libere la memoire allouee aux champs d'une structure */
static void free_champs(int index) { free(trep[index].data.structure.champ); }

/* Libere la memoire allouee aux dimensions d'un tableau */
static void free_dimensions(int index) {
    free(trep[index].data.tableau.dimension);
}

/* Libere la memoire allouee aux parametres d'une fontion ou d'une procedure */
static void free_parametres(int index) {
    free(trep[index].data.fonction.parametre);
}

/* Initialise une nouvelle entree dans la table des representations */
static void init_trep_entree(int index, int nature) {
    trep[index].nature = nature;
    switch (nature) {
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
            break;
    }
}

/* Insere un nouveau champ dans la liste de champs d'une structure */
void trep_ajoute_structure_champ(int index, structure_champ *champ) {
    champ->suivant = trep[index].data.structure.champ;
    trep[index].data.structure.champ = champ;
    trep[index].data.structure.nb_champs++;
}

/* Insere une nouvelle dimension dans la liste de dimensions d'un tableau */
void trep_ajoute_tableau_dimension(int index, tableau_dimension *dimension) {
    dimension->suivant = trep[index].data.tableau.dimension;
    trep[index].data.tableau.dimension = dimension;
    trep[index].data.tableau.nb_dimensions++;
}

/* Insere un nouveau parametre dans la liste de parametres d'une fonction ou d'une procedure */
void trep_ajoute_fonction_param(int index, fonction_param *parametre) {
    parametre->suivant = trep[index].data.fonction.parametre;
    trep[index].data.fonction.parametre = parametre;
    trep[index].data.fonction.nb_parametres++;
}

/* Cree une nouvelle entree dans le tableau des representations */
int trep_nouvelle_entree(int nature) {
    int index = taille_trep;
    init_trep_entree(index, nature);
    taille_trep++;
    return index;
}

/* Affiche la table des representations */
void init_trep() {
    int i;
    for (i = 0; i < TREP_TAILLE_MAX; i++) {
        trep[i].nature = VALEUR_NULL;
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
            case VALEUR_NULL:
                printf("NULL");
                break;
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
                printf("Erreur affichage trep entree.");
                break;
        }
        printf("\n");
    }

    printf(
        "---------------------------------------------------------------------"
        "\n");
}

/* Libère la mémoire allouée au tableau des representations */
void trep_free() {
    int i;
    for (i = 0; i < taille_trep; i++) {
        switch (trep[i].nature) {
            case NATURE_STRUCTURE:
                free_champs(i);
                break;
            case NATURE_TABLEAU:
                free_dimensions(i);
                break;
            case NATURE_PROCEDURE:
            case NATURE_FONCTION:
                free_parametres(i);
                break;
            default:
                break;
        }
    }
}