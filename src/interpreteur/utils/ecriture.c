#include "../../../inc/interpreteur.h"

static void ecrit_str(char type, arbre a, int index_arg) {
    arbre arg = a->fils_gauche;

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
    int index_arg = 0;
    strcpy(str, tlex_recupere_lexeme(a->fils_gauche->valeur_1));
    int i, len = strlen(str);
    char last = '0';
    int delta = 1;
    for (i = 1; i < len - 1; i++) {
        if (last == '\\') {
            delta++;
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
                    ecrit_str(str[i], a->fils_gauche->frere_droit, index_arg);
                    index_arg++;
                    break;
                default:
                    fprintf(stderr, "Erreur - caractere echappe '\\%c' inconnu\n", str[i]);
                    exit(EXIT_FAILURE);
            }
            last = '0';
        } else {
            if (last != '\\' && str[i] == '\\') {
                last = str[i];
                continue;
            } else {
                printf("%c", str[i]);
                if (last == '\\' && str[i] == '\\') {
                    last = '0';

                } else {
                    last = str[i];
                }
            }
        }
    }
    str[len - 1] = '\0';
}