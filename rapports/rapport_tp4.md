# Rapport TP4 — Découpage spatial et Lennard-Jones

## Q1 : Potentiel de Lennard-Jones

Le potentiel de Lennard-Jones est :

U(r_ij) = 4ε [(σ/r_ij)^12 - (σ/r_ij)^6]

- À courte distance (r < σ) : forte répulsion
- À r = 2^(1/6)·σ ≈ 1.12σ : minimum du potentiel (équilibre)
- À grande distance : attraction faible → 0

La force dérivée est :

F_ij = 24ε/r²_ij · (σ/r_ij)^6 · [1 - 2(σ/r_ij)^6] · r_ij

Avec ε=1, σ=1, la distance d'équilibre est ≈ 1.12.

## Q2 : Classe Univers (enrichie)

L'Univers stocke les paramètres physiques (ε, σ, r_cut, dimension, longueurs)
et gère la collection de particules et la grille de cellules.

## Q3 : Grille tensorielle

Le domaine est découpé en cellules de taille r_cut :
- nc_d = floor(L_d / r_cut) cellules par dimension d
- Nombre total : nc_1 × nc_2 × nc_3
- Chaque cellule connaît ses voisines (27 en 3D, 9 en 2D, 3 en 1D)

### Voisinage

Le voisinage est précalculé à l'initialisation. Pour les conditions
périodiques, les cellules des bords sont voisines de celles de l'autre côté.

## Q4 : Calcul des forces avec la grille

```
Pour chaque cellule C :
  Pour chaque paire (i,j) dans C :
    Calculer F_ij si r_ij < r_cut
  Pour chaque cellule voisine V (avec V.index > C.index) :
    Pour chaque i dans C, j dans V :
      Calculer F_ij si r_ij < r_cut
```

L'utilisation de `V.index > C.index` évite le double comptage (3e loi de Newton).

Complexité : O(N) au lieu de O(N²) pour des densités uniformes.

## Q5 : Mise à jour des cellules

À chaque pas de temps, après le déplacement des particules, on recalcule
dans quelle cellule se trouve chaque particule :

```cpp
void Univers::mettreAJourCellules() {
    // Vider toutes les cellules
    // Pour chaque particule, trouverCellule(position)
    // Assigner la particule à la bonne cellule
}
```

## Q6 : Simulation de collision

Paramètres : L1=250, L2=40, ε=5, σ=1, r_cut=2.5, dt=0.00005, tend=19.5
- Rectangle : 160×40 = 6400 particules, statique
- Carré : 40×40 = 1600 particules, v=(0,10)
- Espacement : 2^(1/6)/σ ≈ 1.1225

Résultats : les fichiers VTK montrent la collision et la déformation
des deux objets.
