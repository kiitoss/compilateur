# Compilateur

Ce projet a pour but la création d'un compilateur et d'une machine virtuelle pour le langage CPYRR, un langage procédural à structure de blocs.

<br>

Une première phase de compilation va permettre la construction de tables et d'un arbre binaire tout en effectuant une **_analyse lexicale, synthaxique et sémantique_**.

Ces tables et cet arbre seront ensuite stockés dans un fichier : c'est le **_texte intermédiaire_**

<br>

L'interpreteur est chargé d'executer le programme après
récupération des tables et de l'arbre dans le texte intermédiaire. L'execution se fait par un parcours de l'arbre, chaque noeud parcouru est analysé et une **_pile d'execution_** est générée pour stocker les données.

<br>

## Création de l'executable

```sh
make
```

## Compilation + interprétation

```sh
./compilateur.exe -r fichier
```

## Un exemple

```sh
./compilateur.exe -r ./exemples/representatifs/r-5.txt
```

## Compilation (affichagee de l'aide)

```sh
./compilateur.exe -h
```

## Interprétation (affichagee de l'aide)

```sh
./interpreteur.exe -h
```

<br>

<object data="./presentation.pdf" type="application/pdf" width="700px" height="700px">
    <embed src="./presentation.pdf">
        <p>Votre navigateur ne supporte pas la lecture de PDFs: <a href="./presentation.pdf">Télécharger la présentation</a>.</p>
    </embed>
</object>
