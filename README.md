# Compilateur

## Sections

1. [Explication du projet](#head-1)
2. [Commandes](#head-2)
3. [Exemples d'exécution](#head-3)
4. [Presentation](#head-4)

<br>

## <a name="head-1"></a>Explication du projet

Ce projet a pour but la création d'un compilateur et d'une machine virtuelle pour le langage CPYRR, un langage procédural à structure de blocs.

<br>

Une première phase de compilation va permettre la construction de tables et d'un arbre binaire tout en effectuant une **_analyse lexicale, synthaxique et sémantique_**.

Ces tables et cet arbre seront ensuite stockés dans un fichier : c'est le **_texte intermédiaire_**

<br>

L'interpreteur est chargé d'executer le programme après
récupération des tables et de l'arbre dans le texte intermédiaire. L'exécution se fait par un parcours de l'arbre, chaque noeud parcouru est analysé et une **_pile d'exécution_** est générée pour stocker les données.

<br>

## <a name="head-2"></a>Commandes

### Création de l'executable

```sh
make
```

### Compilation + interprétation

```sh
./compilateur.exe -r fichier
```

### Compilation (affichagee de l'aide)

```sh
./compilateur.exe -h
```

### Interprétation (affichagee de l'aide)

```sh
./interpreteur.exe -h
```

<br>

## <a name="head-3"></a>Exemples d'exécution

### r-5.txt

#### fichier source

```
PROG
    var a : float;
    var b : float;
DEBUT
    a := 1;
    b := 1,2;
    ecrit ("Le resultat de \f + \f caste en entier est \d", a, b, a + b);
FIN
```

#### commande d'exécution

```sh
./compilateur.exe -r ./exemples/representatifs/r-5.txt
```

#### resultat

```
Debut de la compilation
Compilation terminee
Debut de l'interpretation

Le resultat de 1.000000 + 1.200000 caste en entier est 2

Interpretation terminee
```

<br>

### r-14.txt

#### fichier source

```
PROG
var entier : int;
    fonction demande_entier() retourne int
    var a : int;
    DEBUT
        ecrit("Saisissez un entier: ");
        lire(a);
        retourne a;
    FIN;
DEBUT
    entier := demande_entier();
    ecrit("Affichage de l'entier: \d", entier);
FIN
```

#### commande d'exécution

```sh
./compilateur.exe -r ./exemples/representatifs/r-14.txt
```

#### resultat

```
Debut de la compilation
Compilation terminee
Debut de l'interpretation

Saisissez un entier: 4
Affichage de l'entier: 4

Interpretation terminee
```

<br>

## <a name="head-4"></a>Presentation

<object data="./presentation.pdf" type="application/pdf" width="700px" height="700px">
    <embed src="./presentation.pdf">
        <p>Votre navigateur ne supporte pas la lecture de PDFs: <a href="./presentation.pdf">Télécharger la présentation</a>.</p>
    </embed>
</object>
