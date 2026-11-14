/**
 * @file demo_tp2_solar.cxx
 * @brief TP2 — Simulation du système solaire (Soleil, Terre, Jupiter, Halley).
 *
 * Utilise l'algorithme de Störmer-Verlet avec la force gravitationnelle.
 * δt = 0.015, t_end = 468.5
 *
 * Les résultats sont sauvegardés en fichiers VTK (visualisation Paraview)
 * et en fichier texte (positions à chaque pas de temps).
 */

#include "Univers.hxx"
#include <iostream>
#include <fstream>
#include <iomanip>

int main() {
    std::cout << "- TP2 : Système Solaire (Störmer-Verlet) " << std::endl;

    double dt = 0.015;
    double tend = 468.5;

    // Créer l'univers 2D (pas de grille pour la gravitation newtonienne)
    Univers univers(2, {100.0, 100.0}, 0.0, TypeForce::GRAVITATIONNELLE);

    // Soleil : masse=1, position=(0,0), vitesse=(0,0)
    univers.ajouterParticule(Particule(Vecteur(0.0, 0.0), Vecteur(0.0, 0.0), 1.0, 0));
    // Terre : masse=3e-6, position=(0,1), vitesse=(-1,0)
    univers.ajouterParticule(Particule(Vecteur(0.0, 1.0), Vecteur(-1.0, 0.0), 3.0e-6, 1));
    // Jupiter : masse=9.55e-4, position=(0,5.36), vitesse=(-0.425,0)
    univers.ajouterParticule(Particule(Vecteur(0.0, 5.36), Vecteur(-0.425, 0.0), 9.55e-4, 2));
    // Halley : masse=1e-14, position=(34.75,0), vitesse=(0,0.0296)
    univers.ajouterParticule(Particule(Vecteur(34.75, 0.0), Vecteur(0.0, 0.0296), 1.0e-14, 3));

    std::cout << "dt = " << dt << ", t_end = " << tend << std::endl;
    std::cout << "Nombre de particules : " << univers.getNbParticules() << std::endl;

    // Lancer la simulation (sortie VTK automatique)
    univers.simuler(dt, tend, "solar", 500);

    std::cout << "\nSimulation terminée." << std::endl;
    std::cout << "Fichiers VTK générés : solar_*.vtu" << std::endl;
    std::cout << "Visualiser avec Paraview." << std::endl;

    return 0;
}
