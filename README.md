# Chipolata - Moteur d'Échecs Atomic

Un moteur d'échecs implémenté en C, développé **exclusivement pour la variante Atomic** des échecs. Compatible avec le protocole UCI standard pour les interfaces graphiques d'échecs. 
## Compilation

Compilez le projet avec `make` :

```bash
make
```

Cela crée l'exécutable `chipolata`.

## Prérequis et Compatibilité

**Systèmes compatibles :**
- Linux (x86_64, ARM et autres architectures)
- macOS
- Autres systèmes Unix/POSIX

**Non compatible :**
- Windows (actuellement)

**Prérequis Processeur :**

**Le projet requiert l'ensemble d'instructions BMI2 (Bit Manipulation Instruction Set 2)**

Le code utilise les instructions BMI2 via les fonctions `_pext_u64()` et `_pdep_u64()` pour optimiser les opérations sur bitboards. Cela limite la compatibilité aux architectures suivantes :

- **x86-64** : Processeurs Intel Core i5/i7 (4ème génération et plus, ~2013+) ou AMD Ryzen et plus récents
- **ARM / ARM64** : Non supporté (pas d'instruction BMI2)
- **MIPS, PowerPC, RISC-V** : Non supporté
- **Processeurs x86 anciens** : Intel Core 2, Core i3/i5/i7 (Génération 1-3), AMD FX, Phenom II, Atoms anciens

**Note :** Le projet ne propose actuellement **pas d'alternative sans BMI2** pour les architectures non supportées. La compilation échouera sur les systèmes sans instruction BMI2.

**Dépendances Compilateur :**
Le projet utilise les builtins GCC/Clang suivants pour optimiser les opérations sur bitboards :
- `__builtin_ctzll()` - Compte les zéros à droite (Count Trailing Zeros)
- `__builtin_popcountll()` - Compte les bits à 1 (Population Count)

Ces builtins requièrent un compilateur compatible (GCC, Clang, etc.)

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

## Utilisation

Chipolata utilise le protocole UCI standard pour communiquer avec les interfaces graphiques d'échecs compatibles avec la variante Atomic (comme Arena avec plugin Atomic, ChessGUI, etc.).

**Important :** Ce moteur est spécialisé dans les échecs Atomic uniquement. Les règles spécifiques à cette variante (explosion lors d'une capture, perte automatique si le roi est capturé) sont intégrées dans le moteur.

## À propos de la Variante Atomic

Chipolata implémente les règles complètes des échecs Atomic :
- Lors d'une capture, la pièce capturée et toutes les pièces adjacentes au roi adverse sont retirées du jeu
- Le jeu est perdu si le roi adverse explose ou si votre roi est capturé
- Les roques ne sont pas possibles après une capture adjacente au roi
