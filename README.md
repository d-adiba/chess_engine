# Chipolata - Moteur d'Échecs

Un moteur d'échecs simple implémenté en C, utilisant le protocole UCI 
## Compilation

Compilez le projet avec `make` :

```bash
make
```

Cela crée l'exécutable `chipolata`.

## Lancement

Pour lancer le moteur :

```bash
./chipolata
```

Le moteur démarre en mode UCI et attend des commandes du protocole UCI.

## Nettoyage

Pour nettoyer les fichiers compilés :

```bash
make clean
```

## Structure du Projet

- `chipolata.c` - Point d'entrée principal
- `uci.c/uci.h` - Implémentation du protocole UCI
- `bitboard.c/bitboard.h` - Représentation du plateau avec bitboards
- `move.c/move.h` - Générations et calculs des coups
- `attacks.c/attacks.h` - Calcul des attaques
- `random.c/random.h` - Génération de nombres aléatoires
- `Makefile` - Fichier de compilation

## Utilisation

Chipolata utilise le protocole UCI standard pour communiquer avec les interfaces graphiques d'échecs (comme Arena, ChessGUI, etc.).
