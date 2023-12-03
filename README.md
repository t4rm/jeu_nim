# Jeu de Nim

Deux joueurs s’affrontent autour d’une grille quadrillée composée de nlig lignes et de ncol. En début de partie, un pion est placé dans la case de coordonnées (1, 1). 

À tour de rôle, chaque joueur doit déplacer le pion, soit de une ou de deux cases vers la droite,
soit de une ou de deux cases vers le bas. 

Les déplacements autorisés sont tels que le pion termine irrémédiablement sa course en case (nlig, ncol) appelée puits où il se retrouve bloqué.

Le gagnant du jeu est celui qui fera tomber le pion dans le puits.


## Installation

Cloner le projet

```bash
  git clone https://github.com/t4rm/jeu_nim
```

Rendez-vous sur le répertoire du projet

```bash
  cd jeu_nim
```

Compiler le fichier c

```bash
  gcc jeu.c -o jeu
```

Lancer le jeu

```bash
  ./jeu
```


## Crédits
[IUT de Metz - Département Informatique](https://iut-metz.univ-lorraine.fr/departement-informatique)
