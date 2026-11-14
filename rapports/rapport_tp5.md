# Rapport TP5 — Tests, visualisation et analyse ACVL

## Q1-Q2 : Infrastructure de test (Google Test)

Nous utilisons Google Test v1.17.0, intégré via CMake `FetchContent`.

### Suites de tests

| Suite | Fichier | Nombre de tests |
|-------|---------|-----------------|
| VecteurTest | `test/test_vecteur.cxx` | 17 |
| ParticuleTest | `test/test_particule.cxx` | 10 |
| UniversTest | `test/test_univers.cxx` | 10 |
| BoundaryTest | `test/test_boundary.cxx` | 6 |
| **Total** | | **43** |

Tous les tests passent (`ctest --output-on-failure` : 43/43 passed).

### Bonnes pratiques respectées

1. **Tests indépendants et répétables** : chaque test crée ses propres objets
2. **Tests bien organisés** : une suite par classe
3. **Tests portables** : pas de dépendance système
4. **Messages d'erreur clairs** : Google Test affiche le contexte
5. **Exécution rapide** : 0.1s pour tous les tests

## Q3 : Sortie VTK

Le format VTK UnstructuredGrid est utilisé pour sauvegarder les données :

```xml
<VTKFile type="UnstructuredGrid" ...>
  <UnstructuredGrid>
    <Piece NumberOfPoints="N" NumberOfCells="0">
      <Points>
        <DataArray name="Position" ... />
      </Points>
      <PointData>
        <DataArray Name="Velocity" ... />
        <DataArray Name="Masse" ... />
      </PointData>
      <Cells>...</Cells>
    </Piece>
  </UnstructuredGrid>
</VTKFile>
```

La méthode `Univers::ecrireVTK()` génère des fichiers numérotés
(prefix_000001.vtu, etc.) visualisables avec Paraview.

## Q4-Q7 : Diagrammes ACVL

Les diagrammes sont détaillés dans `doc/analyse.txt` :

### Cas d'utilisation
- Configurer une simulation
- Lancer une simulation
- Visualiser les résultats
- Tester le code
- Générer la documentation

### Séquence (simulation)
Utilisateur → main → Univers → Particule (boucle temporelle)

### États-transitions
Initialisé → Particules ajoutées → Grille construite → Simulation → Terminée

### Classes d'analyse
- **Vecteur** : type fondamental pour les coordonnées
- **Particule** : entité physique avec position, vitesse, force
- **Cellule** : élément de la grille spatiale
- **Univers** : conteneur principal, gère la simulation
