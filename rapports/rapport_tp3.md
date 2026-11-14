# Rapport TP3 — Opérateurs et classe Univers

## Q1-Q2 : Classe Vecteur

La classe `Vecteur` (dimension 3) est définie dans `include/Vecteur.hxx` et
implémentée dans `src/Vecteur.cxx`.

### Opérateurs implémentés

| Opérateur | Type | Description |
|-----------|------|-------------|
| `+` | binaire libre | Addition vectorielle |
| `-` | binaire libre | Soustraction vectorielle |
| `*` | binaire libre | Multiplication scalaire (deux sens) |
| `/` | binaire libre | Division scalaire |
| `-` | unaire libre | Négation |
| `+=` | membre | Addition composée |
| `-=` | membre | Soustraction composée |
| `*=` | membre | Multiplication composée |
| `/=` | membre | Division composée |
| `==`, `!=` | libre | Comparaison (tolérance 1e-12) |
| `<<` | libre | Flux de sortie |
| `=` | membre | Affectation |

### Méthodes

- `norme()` : norme euclidienne √(x² + y² + z²)
- `normeCarree()` : x² + y² + z² (évite le sqrt)
- `dot()` : produit scalaire
- `cross()` : produit vectoriel

## Q3 : Tests

17 tests unitaires pour la classe Vecteur dans `test/test_vecteur.cxx`.

## Q4 : Modification de la classe Particule

La classe `Particule` utilise maintenant `Vecteur` pour la position, la vitesse
et la force. Cela simplifie considérablement le code de simulation.

## Q5-Q6 : Classe Univers

La classe `Univers` gère :
- La collection de particules (`std::vector<Particule>`)
- La dimension (1D, 2D ou 3D)
- Le calcul des forces d'interaction
- L'algorithme de Störmer-Verlet
- La sortie des résultats

## Q7-Q8 : Performances

Testé avec (2^k)³ particules, k = 3..7.
Le calcul des forces est en O(N²) sans optimisation.

## Q9 : Optimisation ×2 — 3e loi de Newton

**F_ij = -F_ji** : en calculant la force entre (i,j) une seule fois et en
l'appliquant symétriquement, on divise le nombre de calculs par 2.

```cpp
for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {  // j = i+1, pas j = 0
        Vecteur fij = calculerForce(i, j);
        particules[i].ajouterForce(fij);
        particules[j].ajouterForce(-fij);  // 3e loi de Newton
    }
}
```

## Q10 : Autres optimisations possibles

1. **Découpage spatial** (TP4) : réduire la complexité de O(N²) à O(N) via
   une grille et un rayon de coupure.
2. **Parallélisme** : OpenMP pour la boucle de calcul des forces.
