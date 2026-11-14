# Guide de Contribution

Merci de l'intérêt porté au projet **ParticleSimulation**. Ce document définit les normes et directives pour contribuer efficacement et maintenir la rigueur technique du code.

---

## 1. Principes Généraux

- **Rigueur scientifique** : Tout algorithme numérique doit être documenté mathématiquement et validé par des tests unitaires.
- **Sobriété et clarté** : Privilégier un code lisible, modulaire et auto-documenté. Éviter toute complexité superflue.
- **Tolérance zéro fuite mémoire** : Tous les exécutables et tests doivent passer `valgrind` sans aucune fuite ni accès invalide.

---

## 2. Environnement de Développement

### Prérequis

- Compilateur C++ supportant **C++17** (`g++ >= 9.0` ou `clang++ >= 10.0`)
- **CMake >= 3.16.3**
- **Make** ou **Ninja**
- Outils recommandés : `valgrind`, `doxygen`, `graphviz`, `clang-format`

### Compilation en mode Développement

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DBUILD_DEMOS=ON
cmake --build build -j$(nproc)
```

### Exécution des tests

```bash
ctest --test-dir build --output-on-failure
```

---

## 3. Workflow Git

### Branches

- `main` : Branche stable, protégée, reflétant l'état de production.
- `feature/<nom-fonctionnalite>` : Nouvelles fonctionnalités ou simulations.
- `fix/<nom-correctif>` : Résolution de bogues.
- `docs/<description>` : Modifications de la documentation.

### Format des Messages de Commit

Les commits doivent suivre la spécification **Conventional Commits** :

```
<type>(<portée>): <description concise en minuscules>

[corps optionnel détaillant le contexte technique]

[référence optionnelle aux issues : Fixes #123]
```

Types acceptés :
- `feat` : Nouvelle fonctionnalité
- `fix` : Correction de bogue
- `test` : Ajout ou modification de tests unitaires
- `docs` : Documentation (Markdown, Doxygen)
- `refactor` : Réorganisation du code sans modification de comportement
- `perf` : Optimisation de performance
- `build` : Modification du système de build (CMake)
- `ci` : Configuration de l'intégration continue

Exemple :
```bash
feat(univers): add velocity Verlet integration with domain decomposition
test(boundary): add test cases for potential-based reflective walls
docs(readme): add ParaView visualization tutorial and mathematical equations
```

---

## 4. Normes de Codage

- **Standard** : C++17 strict (`-Wall -Wextra -Wpedantic`).
- **Formatage** : Utiliser le fichier `.clang-format` fourni à la racine.
  ```bash
  clang-format -i include/*.hxx src/*.cxx demo/*.cxx test/*.cxx
  ```
- **Conventions de nommage** :
  - Classes : `CamelCase` (`Univers`, `Particule`, `Vecteur`)
  - Méthodes et fonctions : `camelCase` (`calculerForces()`, `energieCinetique()`)
  - Variables membres : préfixe `m_` (`m_position`, `m_vitesse`)
  - Constantes et énumérations : `UPPER_SNAKE_CASE` (`TypeForce::GRAVITATIONNELLE`)
  - Fichiers d'en-tête : extension `.hxx`
  - Fichiers sources : extension `.cxx`
- **Documentation Doxygen** :
  - Chaque classe et méthode publique doit comporter un bloc de documentation Doxygen (`@brief`, `@param`, `@return`).

---

## 5. Critères d'Acceptation d'une Pull Request

Avant de soumettre une Pull Request :

1. Le projet doit compiler sans aucun avertissement (`-Wall -Wextra`).
2. Tous les tests GoogleTest existants et nouveaux doivent passer avec succès (`ctest`).
3. La documentation Doxygen doit se générer sans avertissement (`cmake --build build --target doc`).
4. L'analyse mémoire `valgrind` ne doit signaler aucune fuite (`0 errors from 0 contexts`).
5. La description de la PR doit expliciter clairement les modifications apportées et la méthodologie de test adoptée.
