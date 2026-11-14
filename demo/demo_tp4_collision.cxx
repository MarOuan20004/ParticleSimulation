/**
 * @file demo_tp4_collision.cxx
 * @brief TP4 — Simulation de collision entre deux objets (Lennard-Jones).
 *
 * Configuration :
 *   L1 = 250, L2 = 40
 *   ε = 5, σ = 1
 *   m = 1, v = (0, 10) pour le carré
 *   N1 = 1600 (carré 40×40), N2 = 6400 (rectangle 160×40)
 *   r_cut = 2.5σ, δt = 0.0005
 *   Simulation jusqu'à t = 19.5
 *
 *   Distance d'équilibre LJ : 2^(1/6) * σ ≈ 1.1225
 */

#include "Univers.hxx"
#include <iostream>
#include <cmath>

int main() {
    std::cout << "TP4 : Collision de deux objets (Lennard-Jones)" << std::endl;

    double L1 = 250.0;
    double L2 = 40.0;
    double epsilon = 5.0;
    double sigma = 1.0;
    double rcut = 2.5 * sigma;
    double dt = 0.05;
    double tend = 19.5;
    double masse = 1.0;
    double spacing = std::pow(2.0, 1.0 / 6.0) * sigma;

    // Créer l'univers 2D
    Univers univers(2, {L1, L2}, rcut, TypeForce::LENNARD_JONES);
    univers.setLennardJonesParams(epsilon, sigma);

    // Rectangle : 160 × 40 particules, statique (v=0)
    // Positionné au centre-gauche du domaine
    double x0_rect = 20.0;
    double y0_rect = (L2 - 40 * spacing) / 2.0;
    for (int ix = 0; ix < 160; ++ix) {
        for (int iy = 0; iy < 40; ++iy) {
            Vecteur pos(x0_rect + ix * spacing, y0_rect + iy * spacing);
            univers.ajouterParticule(Particule(pos, Vecteur(0.0, 0.0), masse, 0));
        }
    }

    // Carré : 40 × 40 particules, vitesse = (0, 10)
    // Positionné à droite du rectangle, collision latérale
    double x0_carre = x0_rect + 160 * spacing + 5.0 ;
    double y0_carre = (L2 - 40 * spacing) / 2.0;

    for (int ix = 0; ix < 40; ++ix) {
        for (int iy = 0; iy < 40; ++iy) {
            Vecteur pos(x0_carre + ix * spacing, y0_carre + iy * spacing);
            Vecteur vit(0.0, 10.0);
            univers.ajouterParticule(Particule(pos, vit, masse, 1));
        }
    }

    std::cout << "Nombre total de particules : " << univers.getNbParticules() << std::endl;
    std::cout << "dt = " << dt << ", t_end = " << tend << std::endl;
    std::cout << "epsilon = " << epsilon << ", sigma = " << sigma << std::endl;
    std::cout << "r_cut = " << rcut << std::endl;
    std::cout << "Espacement = " << spacing << std::endl;

    // Initialiser la grille spatiale
    univers.initialiserGrille();

    // Lancer la simulation
    int outputInterval = static_cast<int>(0.5 / dt); // Sortie toutes les 0.5 unités de temps
    univers.simuler(dt, tend, "collision_tp4", outputInterval);

    std::cout << "\nSimulation terminée." << std::endl;
    std::cout << "Fichiers VTK générés : collision_tp4_*.vtu" << std::endl;

    return 0;
}
