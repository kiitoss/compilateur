#include "../../../inc/interpreteur.h"

/*
 * Resolution d'un affichage sur la sortie standard
 */
void resout_ecrit(arbre a) {
    char *str = tlex_recupere_lexeme(a->fils_gauche->valeur_1);
    int i, len = strlen(str);
    char last = '0';
    int delta = 1;
    for (i = 1; i < len - 1; i++) {
        if (last == '\\') {
            delta++;
            switch (str[i]) {
                case 'n':
                    str[i - delta] = '\n';
                    break;
                case 't':
                    str[i - delta] = '\t';
                    break;
                default:
                    fprintf(stderr, "Erreur - caractere echappe '\\%c' inconnu\n", str[i]);
                    exit(EXIT_FAILURE);
            }
            last = '0';
        } else {
            str[i - delta] = str[i];
            last           = str[i];
        }
    }
    str[i - delta] = '\0';
    fprintf(stdout, "%s", str);
}