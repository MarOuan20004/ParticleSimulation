# Rapport TP1 — Débogage et résolution d'EDO

## 1. Débogage du programme `trace.cxx`

### Q1 : Problèmes identifiés

1. **`calloc` sans cast** : En C++, `calloc` retourne `void*` qui ne se convertit pas
   implicitement en `double**` ou `double*`. Remplacé par `new`.

2. **Nom de fonction incohérent** : Déclarée `fill_vectors` mais définie `fill_vector`.

3. **Boucle `for (i = 1; i < n; i++)`** dans `main()` : La première ligne (i=0) n'est jamais
   remplie. Corrigé en `i = 0`.

4. **Dépassement de bornes dans `trace()`** : `for (i = 1; i <= n; ...)` accède à
   `matrix[n][n]` qui est hors limites. Corrigé en `for (i = 0; i < n; ...)`.

5. **Retour de pointeur vers variable locale** : `trace()` retourne `&sum` où `sum` est
   locale → comportement indéfini. Corrigé en retournant `double` par valeur.

6. **Retour `&vec` dans `fill_vector`** : Retourne l'adresse d'un pointeur local.
   Corrigé : la fonction ne retourne plus rien (void).

7. **Fuite mémoire** : La matrice n'est jamais libérée. Ajouté `free_matrix()`.

8. **`endl` sans `std::`** : Ajouté le namespace.

### Q2 : Corrections

Voir `demo/demo_tp1_trace.cxx` pour le code corrigé.

### Q3 : Profiling

Compilé avec `-pg`, exécuté avec `gprof`. Pour des matrices de taille > 1000,
la majeure partie du temps est passée dans `fill_vector` et `trace`.

### Q4 : Optimisation

Le parcours de la matrice est déjà en ligne (row-major), ce qui est optimal
pour le cache. Pour de très grandes matrices, on pourrait utiliserle parallélisme OpenMP.

## 2. Résolution d'EDO

### Euler explicite

Schéma : u_{n+1} = u_n + h · φ(x_n, u_n)

Convergence d'ordre 1 vérifiée sur le cas test u' = 2xu :

| N     | h       | Erreur Euler explicite |
|-------|---------|----------------------|
| 10    | 1e-1    | ~0.27                |
| 100   | 1e-2    | ~0.027               |
| 1000  | 1e-3    | ~0.0027              |
| 10000 | 1e-4    | ~0.00027             |

### Euler implicite

Schéma : u_{n+1} = u_n + h · φ(x_{n+1}, u_{n+1})

Résolu par itération de point fixe.

### Cas raide (u' = -50(u - cos(x)))

- **Euler explicite** : instable pour h > 2/50 = 0.04 (N < 50).
  Nécessite N ≥ 100 pour la stabilité.
- **Euler implicite** : stable pour tous les pas de temps (A-stable).

Cela illustre l'avantage de l'Euler implicite pour les problèmes raides.
