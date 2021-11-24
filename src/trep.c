#include "inc/trep.h"

#include <stdio.h>
#include <stdlib.h>

int index_global_trep = VALEUR_NULL;

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

/* Initialise une nouvelle entree dans le tebleau des representations */
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

/* Met à jour l'index global de la table des representations sur lequelle le travail s'effectue */
void set_index_global_trep(int index) {
    index_global_trep = index;
}

/* Retourne l'index global de la table des representations sur lequelle le travail s'effectue */
int get_global_index_trep() {
    return index_global_trep;
}

/* Remet à zero l'index global de la table des representations sur laquelle le travail s'effectue */
void reinitialise_index_global_trep() {
    index_global_trep = VALEUR_NULL;
}

/* Retourne 1 si index global de la table des representations est null, 0 sinon */
int est_null_index_global_trep() {
    return index_global_trep == VALEUR_NULL;
}

/* Insere un nouveau champ dans la liste de champs d'une structure */
void trep_ajoute_structure_champ(int index, int type, int num_lex, int exec) {
    structure_champ *champ = malloc(sizeof(structure_champ));
    champ->type = type;
    champ->num_lex = num_lex;
    champ->exec = exec;
    champ->suivant = trep[index].data.structure.champ;
    trep[index].data.structure.champ = champ;
    trep[index].data.structure.nb_champs++;
}

/* Insere une nouvelle dimension dans la liste de dimensions d'un tableau */
void trep_ajoute_tableau_dimension(int index, int borne_inf, int borne_sup) {
    tableau_dimension *dimension = malloc(sizeof(tableau_dimension));
    dimension->borne_inf = borne_inf;
    dimension->borne_sup = borne_sup;
    dimension->suivant = trep[index].data.tableau.dimension;
    trep[index].data.tableau.dimension = dimension;
    trep[index].data.tableau.nb_dimensions++;
}

/* Insere un nouveau parametre dans la liste de parametres d'une fonction ou d'une procedure */
void trep_ajoute_fonction_param(int index, int num_lex, int type) {
    fonction_param *parametre = malloc(sizeof(fonction_param));
    parametre->num_lex = num_lex;
    parametre->type = type;
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
    int i, affiche_marque = 1;
    printf(
        "---------------------------------------------------------------------"
        "\nindice\t|\tnature\t\t|\tdata\n");

    for (i = 0; i < TREP_TAILLE_MAX; i++) {
        if (trep[i].nature == VALEUR_NULL) {
            if (affiche_marque) {
                affiche_marque = 0;
                printf("............\n");
            }
            continue;
        }
        affiche_marque = 1;
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