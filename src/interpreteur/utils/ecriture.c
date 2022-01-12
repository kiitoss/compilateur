#include "../../../inc/interpreteur.h"

static void ecrit_str(char type, arbre a, int index_arg) {
    arbre arg = a->fils_gauche;

    /* positionnement sur le bon argument (= arg n°index_arg) */
    if (arbre_est_vide(arg) || arbre_recupere_nature(arg) != A_ARG) {
        fprintf(stderr, "Erreur - 0 argument\n");
        exit(EXIT_FAILURE);
    }
    for (int j = 0; j < index_arg; j++) {
        arg = arg->frere_droit;
        if (arbre_est_vide(arg) || arbre_recupere_nature(arg) != A_ARG) {
            fprintf(stderr, "Erreur - argument manquant\n");
            exit(EXIT_FAILURE);
        }
    }

    /* affichage de la valeur selon l'affichage demande par l'utilisateur */
    switch (type) {
        case 'd':
            printf("%d", resout_expression(arg->fils_gauche, 0).entier);
            break;
        case 'f':
            printf("%f", resout_expression(arg->fils_gauche, 1).reel);
            break;
        case 'c':
            printf("%c", resout_expression(arg->fils_gauche, 3).caractere);
            break;
        default:
            fprintf(stderr, "Cas type ecriture variable non gere\n");
            exit(EXIT_FAILURE);
    }
}

/*
 * Resolution d'un affichage sur la sortie standard
 */
void resout_ecriture(arbre a) {
    char str[100];
    int len, index_arg = 0, echappe = 0;

    strcpy(str, tlex_recupere_lexeme(a->fils_gauche->valeur_1));
    len = strlen(str);

    for (int i = 1; i < len - 1; i++) {
        /* si le caractere precedant etait un back-slash */
        if (echappe) {
            switch (str[i]) {
                case 'n':
                    printf("\n");
                    break;
                case 't':
                    printf("\t");
                    break;
                case 'd':
                case 'f':
                case 'c':
                    /* ecrit la partie droite de l'arbre sur la sortie standart */
                    ecrit_str(str[i], a->fils_gauche->frere_droit, index_arg);
                    index_arg++;
                    break;
                default:
                    fprintf(stderr, "Erreur - caractere echappe '\\%c' inconnu\n", str[i]);
                    exit(EXIT_FAILURE);
            }
            echappe = 0;
        } else {
            if (str[i] == '\\') {
                echappe = 1;
                continue;
            }
            printf("%c", str[i]);
        }
    }
}