/**
 * @file demo_tp6_boundary.cxx
 * @brief TP6 — Conditions aux limites + gravité + rescaling d'énergie.
 *
 * Configuration :
 *   L1 = 250, L2 = 180
 *   ε = 1, σ = 1
 *   m = 1, v = (0, 10) pour le carré
 *   N1 = 395 (carré), N2 = 17227 (rectangle)
 *   r_cut = 2.5σ, δt = 0.0005
 *   G = -12
 *   EcD = 0.005 * (N1 + N2)
 *   Rescale toutes les 1000 itérations
 *   Condition aux limites : réflexion par potentiel
 *   Simulation jusqu'à t = 29.5
 */

#include "Univers.hxx"
#include <iostream>
#include <cmath>

int main() {
    std::cout << "=== TP6 : Conditions aux limites + gravité ===" << std::endl;

    double L1 = 250.0;
    double L2 = 180.0;
    double epsilon = 1.0;
    double sigma = 1.0;
    double rcut = 2.5 * sigma;
    double dt = 0.0005;
    double tend = 29.5;
    double masse = 1.0;
    double G = -12.0;
    double spacing = std::pow(2.0, 1.0 / 6.0) * sigma;

    // Déterminer les tailles pour atteindre N1≈395 et N2≈17227
    // Rectangle : trouvons nx, ny tels que nx*ny ≈ 17227
    // En utilisant un ratio similaire au TP4 (160×40 → 4:1)
    // sqrt(17227 / 4) ≈ 65.6, donc environ 131 × 131 ou 262 × 66
    int nx_rect = 131;
    int ny_rect = 131;
    // Ajustons : 131*131 = 17161 ≈ 17227

    // Carré : sqrt(395) ≈ 19.9, donc 20×20 = 400 ≈ 395
    int n_carre = 20;

    int N1 = n_carre * n_carre;
    int N2 = nx_rect * ny_rect;
    double EcD = 0.005 * (N1 + N2);

    // Créer l'univers 2D
    Univers univers(2, {L1, L2}, rcut, TypeForce::LENNARD_JONES);
    univers.setLennardJonesParams(epsilon, sigma);
    univers.setGravite(G);
    univers.setRescaleEnergie(EcD, 1000);
    univers.setConditionLimite(ConditionLimite::REFLEXION, true); // Réflexion par potentiel

    // Rectangle : particules statiques
    double x0_rect = (L1 - nx_rect * spacing) / 2.0;
    double y0_rect = 10.0; // Positionné en bas du domaine
    for (int ix = 0; ix < nx_rect; ++ix) {
        for (int iy = 0; iy < ny_rect; ++iy) {
            Vecteur pos(x0_rect + ix * spacing, y0_rect + iy * spacing);
            univers.ajouterParticule(Particule(pos, Vecteur(0.0, 0.0), masse, 0));
        }
    }

    // Carré : vitesse = (0, 10), positionné au-dessus
    double x0_carre = (L1 - n_carre * spacing) / 2.0;
    double y0_carre = y0_rect + ny_rect * spacing + 5.0;
    for (int ix = 0; ix < n_carre; ++ix) {
        for (int iy = 0; iy < n_carre; ++iy) {
            Vecteur pos(x0_carre + ix * spacing, y0_carre + iy * spacing);
            Vecteur vit(0.0, 10.0);
            univers.ajouterParticule(Particule(pos, vit, masse, 1));
        }
    }

    std::cout << "Nombre total de particules : " << univers.getNbParticules() << std::endl;
    std::cout << "  N1 (carré) = " << N1 << ", N2 (rectangle) = " << N2 << std::endl;
    std::cout << "dt = " << dt << ", t_end = " << tend << std::endl;
    std::cout << "G = " << G << ", EcD = " << EcD << std::endl;
    std::cout << "Condition aux limites : réflexion par potentiel" << std::endl;

    // Initialiser la grille
    univers.initialiserGrille();

    // Lancer la simulation
    int outputInterval = static_cast<int>(1.0 / dt); // Sortie toutes les 1.0 unités de temps
    univers.simuler(dt, tend, "boundary_tp6", outputInterval);

    std::cout << "\nSimulation terminée." << std::endl;
    std::cout << "Fichiers VTK générés : boundary_tp6_*.vtu" << std::endl;

    return 0;
}
