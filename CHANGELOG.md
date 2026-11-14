# Journal des Modifications (Changelog)

Toutes les modifications notables apportées à ce projet sont documentées dans ce fichier.
Le format est basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.0.0/) et ce projet adhère à [Semantic Versioning](https://semver.org/lang/fr/).

---

## [1.0.0] - 2026-05-07

### Ajouté
- **Module Algèbre Vectorielle 3D (`Vecteur`)** :
  - Opérateurs arithmétiques binaires et composés (`+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`).
  - Produit scalaire (`dot`), produit vectoriel (`cross`), norme euclidienne et norme au carré (`normeCarree`).
  - Opérateurs de comparaison avec tolérance numérique (`1e-12`) et opérateur de flux de sortie (`<<`).
- **Module Dynamique Particulaire (`Particule`)** :
  - Modélisation de particules 3D avec position, vitesse, force courante, force précédente, masse et catégorie.
  - Attribution automatique d'identifiants uniques via compteur statique thread-safe.
  - Calcul d'énergie cinétique individuelle.
- **Module Découpage Spatial (`Cellule`)** :
  - Décomposition spatiale du domaine en cellules cubiques pour recherche des voisins dans le rayon de coupure ($r_{cut}$).
  - Réduction de la complexité algorithmique des forces de $O(N^2)$ à $O(N)$.
- **Module Univers de Simulation (`Univers`)** :
  - Intégrateur symplectique de Störmer-Verlet (vitesse de Verlet) pour la conservation de l'énergie mécanique.
  - Interaction gravitationnelle newtonienne ($N$-corps).
  - Potentiel de Lennard-Jones 12-6 avec coupure nette ($r_{cut} = 2.5\sigma$).
  - Optimisation par la 3e loi de Newton ($F_{ij} = -F_{ji}$).
  - Conditions aux limites configurables : réflexion géométrique, réflexion par potentiel répulsif aux parois, absorption, et conditions périodiques (tore).
  - Champ gravitationnel externe uniforme ($G$).
  - Thermostat numérique par rééchelonnement des vitesses (*velocity rescaling*).
  - Exportateur au format VTK XML Unstructured Grid (`.vtu`) pour rendu 3D haute fidélité dans ParaView.
- **Démonstrations Scientifiques (`demo/`)** :
  - `demo_tp1_trace` : Débogage et calcul de trace matricielle.
  - `demo_tp1_euler` : Comparaison Euler explicite / Euler implicite sur EDO régulières et raides.
  - `demo_tp2_solar` : Simulation du système solaire (Soleil, Terre, Lune, comète) avec conservation d'énergie.
  - `demo_tp3_collections` : Benchmark comparatif des conteneurs STL (`std::vector`, `std::list`, `std::deque`) et profil de complexité $O(N^2)$.
  - `demo_tp4_collision` : Collision moléculaire 2D entre deux corps cristallins (Lennard-Jones) avec grille spatiale.
  - `demo_tp6_boundary` : Système sous gravité externe avec réflexion par potentiel répulsif et thermostat cinétique.
- **Suite de Tests Unitaires (`test/`)** :
  - 43 cas de tests GoogleTest couvrant l'intégralité des modules (`Vecteur`, `Particule`, `Univers`, `Boundary`).
  - Couverture fonctionnelle à 100% avec validation mémoire Valgrind (0 fuite, 0 erreur).
- **Documentation & Rapports** :
  - Rapports techniques exhaustifs pour chaque phase (`rapports/rapport_tp1.md` à `rapport_tp6.md`).
  - Diagrammes d'analyse UML (cas d'utilisation, séquence, états-transitions, classes) dans `doc/analyse.txt`.
  - Configuration Doxygen complète générant une documentation HTML détaillée.
