## Description des Modifications

Résumé concis des changements introduits par cette Pull Request.

### Type de modification
- [ ] Correction de bogue (non-breaking change résolvant une anomalie)
- [ ] Nouvelle fonctionnalité (non-breaking change ajoutant une capacité)
- [ ] Optimisation de performance (algorithmique ou bas niveau)
- [ ] Amélioration de la documentation ou des tests
- [ ] Modification cassante (breaking change modifiant une interface publique)

### Contexte technique et algorithmique
- Modules impactés : `include/`, `src/`, `demo/`, `test/`, `doc/`
- Justification des choix mathématiques / numériques :

### Liste de contrôle de validation
- [ ] Le code respecte le standard C++17 et compile sans avertissement (`-Wall -Wextra`).
- [ ] Le formatage est conforme à `.clang-format`.
- [ ] De nouveaux tests unitaires ont été ajoutés pour couvrir les modifications.
- [ ] Tous les tests passent avec succès (`ctest --output-on-failure`).
- [ ] Aucune fuite mémoire n'est détectée par `valgrind`.
- [ ] La documentation Doxygen a été mise à jour et se compile sans avertissement.
