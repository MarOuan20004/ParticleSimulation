# Rapport TP2 — Classe Particule et Störmer-Verlet

## Q1 : Classe Particule

La classe `Particule` est définie dans `include/Particule.hxx` et implémentée
dans `src/Particule.cxx`. Elle encapsule :

- **Position** (Vecteur 3D)
- **Vitesse** (Vecteur 3D)
- **Force** et **ForceOld** (pour Störmer-Verlet)
- **Masse** (double)
- **Identifiant** (auto-incrémenté via compteur statique)
- **Catégorie** (type de particule)

## Q2 : Collection de particules

Nous utilisons `std::vector<Particule>` comme collection principale car :
- Accès aléatoire O(1)
- Localité mémoire (cache-friendly)
- Pré-allocation possible avec `reserve()`

## Q3 : Comparaison des collections

Voir `demo/demo_tp3_collections.cxx` pour les benchmarks.
Les différences s'observent à partir de ~512 particules :
- `vector` : le plus rapide pour le parcours séquentiel (contiguïté mémoire)
- `list` : le plus lent (allocations dispersées, défauts de cache)
- `deque` : intermédiaire

## Q4 : Algorithme de Störmer-Verlet

Implémenté avec les méthodes `updatePosition()` et `updateVitesse()` directement
dans la classe `Particule` (pas dans `Univers` ou `main()`).

```
1. Calculer les forces F
2. Pour t < t_end :
   a. Pour chaque particule i :
      x_i = x_i + dt * (v_i + 0.5/m_i * F_i * dt)
      F_old_i = F_i
   b. Recalculer les forces F
   c. Pour chaque particule i :
      v_i = v_i + dt * 0.5/m_i * (F_i + F_old_i)
```

## Q5 : Système solaire

Paramètres :
- Soleil : m=1, pos=(0,0), v=(0,0)
- Terre : m=3e-6, pos=(0,1), v=(-1,0)
- Jupiter : m=9.55e-4, pos=(0,5.36), v=(-0.425,0)
- Halley : m=1e-14, pos=(34.75,0), v=(0,0.0296)
- dt=0.015, t_end=468.5

La simulation produit des orbites cohérentes :
- Terre : orbite quasi-circulaire
- Jupiter : orbite circulaire plus large
- Halley : orbite elliptique très excentrique

## Q6 : Protection des données

Les données de la classe `Particule` sont en `private` avec des accesseurs
`getX()` / `setX()` pour contrôler l'accès en lecture/écriture.
