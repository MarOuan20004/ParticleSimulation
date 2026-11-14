# Rapport TP6 — Conditions aux limites et raffinement

## Q1 : Conditions aux limites

### Réflexion (simple)
La particule rebondit sur la paroi. Les composantes de vitesse perpendiculaires
à la paroi changent de signe.

```
Si x < 0 :  x = -x,  vx = -vx
Si x > L :  x = 2L - x,  vx = -vx
```

### Absorption
La particule est supprimée de la simulation lorsqu'elle sort du domaine.

### Périodique
La particule est transportée sur la paroi opposée sans modification de vitesse.

```
x = fmod(x, L)
Si x < 0 : x += L
```

## Q2 : Tests des conditions aux limites

6 tests unitaires dans `test/test_boundary.cxx` :
- Réflexion bord supérieur et inférieur
- Absorption (vérification de la suppression)
- Périodique (positif et négatif)
- Rescaling des vitesses

## Q3 : Réflexion par potentiel

Le potentiel aux parois est défini par :

F_i = -24ε / (2r) · (σ/(2r))^6 · [1 - 2(σ/(2r))^6]

avec r la distance au mur et r_cut = 2^(1/6)·σ.

Ce potentiel crée une force répulsive qui empêche les particules de
traverser les parois, de manière plus physique que la réflexion simple.

## Q4 : Comparaison des deux implémentations

- **Réflexion simple** : plus rapide, mais non physique (discontinuité de vitesse)
- **Réflexion par potentiel** : plus réaliste, forces continues, mais nécessite
  un pas de temps suffisamment petit.

## Q5 : Champ gravitationnel

Force uniforme appliquée à chaque particule : F_G = (0, m_i × G)

Implémentée dans `Univers::appliquerGravite()`.

## Q6 : Simulation TP6

Paramètres :
- L1=250, L2=180, ε=1, σ=1, r_cut=2.5, dt=0.0005, tend=29.5
- G=-12
- EcD = 0.005 × (N1 + N2), rescale toutes les 1000 itérations
- Réflexion par potentiel

## Q7-Q8 : Gestion des erreurs

### Mécanisme choisi : exceptions C++

```cpp
if (dimension < 1 || dimension > 3)
    throw std::invalid_argument("dimension doit être 1, 2 ou 3");
if (i < 0 || i >= n)
    throw std::out_of_range("indice hors limites");
if (scalar == 0.0)
    throw std::runtime_error("division par zéro");
```

### Forces
- Mécanisme standard C++ (pas de dépendance externe)
- Messages d'erreur explicites
- Propagation automatique dans la pile d'appels
- Types d'exception spécifiques (`invalid_argument`, `out_of_range`, `runtime_error`)

### Faiblesses
- Coût en performance (déroulement de la pile)
- Ne peut pas gérer les erreurs de segmentation
- Les exceptions non attrapées terminent le programme
- Alternative possible : codes de retour (style C) ou `std::optional`/`std::expected`
