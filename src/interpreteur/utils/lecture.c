#include "../../../inc/interpreteur.h"

/*
 * Resolution d'une lecture sur l'entree standard
 */
void resout_lecture(arbre a) {
    int tlex_index, tdec_index, tdec_index_type;
    int scanf_res = 1;
    cellule c     = cellule_null();
    for (arbre var = a->fils_gauche->fils_gauche; !arbre_est_vide(var) && arbre_recupere_nature(var) == A_VAR;
         var       = var->frere_droit) {
        tlex_index      = var->valeur_1;
        tdec_index      = tdec_trouve_index(tlex_index, PREG);
        tdec_index_type = tdec_recupere_description(tdec_index);
        switch (tdec_index_type) {
            case 0:
                scanf_res = scanf("%d", &c.entier);
                break;
            case 1:
                scanf_res = scanf("%f", &c.reel);
                break;
            case 2:
                scanf_res = scanf("%d", &c.booleen);
                break;
            case 3:
                scanf_res = scanf("%c", &c.caractere);
                break;
            default:
                fprintf(stderr, "Erreur - lecture type inconnu (tlex: %d, type: %d)\n", tlex_index, tdec_index_type);
                exit(EXIT_FAILURE);
        }

        if (scanf_res != 1) {
            fprintf(stderr, "Erreur - Scanf a echoue\n");
            exit(EXIT_FAILURE);
        }

        PEXEC[pexec_index_variable(tlex_index, tdec_recupere_taille_exec(tlex_index))] = c;
    }
}