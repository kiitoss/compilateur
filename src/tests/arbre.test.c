#include <stdlib.h>
#include <stdio.h>
#include "../arbre.h"

int main(){
  
  arbre d = creeArbre(1,creeArbre(2,creeNoeud(3),creeNoeud(4)),creeArbre(5,creeNoeud(6),creeNoeud(7)));
  //afficheArbre(d);

  arbre a = creeArbre(8,creeArbreVide(),creeNoeud(9));
  d = insereFilsGauche(d ,a);
  
  afficheArbre(d);
  
  return EXIT_SUCCESS;

}