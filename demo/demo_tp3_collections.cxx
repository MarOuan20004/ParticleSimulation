// demo_tp3_collections.cxx
// TP3 : comparaison des performances des collections STL
// vector vs list vs deque pour l'insertion et le parcours
// + benchmark du calcul d'interactions

#include "Vecteur.hxx"
#include "Particule.hxx"
#include "Univers.hxx"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <list>
#include <deque>
#include <random>

// cree une particule aleatoire dans [0,1]^3
Particule creerParticuleAleatoire(std::mt19937& gen) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Vecteur pos(dist(gen), dist(gen), dist(gen));
    Vecteur vit(dist(gen) - 0.5, dist(gen) - 0.5, dist(gen) - 0.5);
    return Particule(pos, vit, 1.0, 0);
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << "=== TP3 : Comparaison des performances des collections ===" << std::endl;
    std::cout << std::setw(10) << "N"
              << std::setw(15) << "vector(ms)"
              << std::setw(15) << "list(ms)"
              << std::setw(15) << "deque(ms)" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    for (int N = 64; N <= 4096; N *= 2) {
        // std::vector
        auto t0 = std::chrono::steady_clock::now();
        {
            std::vector<Particule> vec;
            for (int i = 0; i < N; ++i) {
                vec.push_back(creerParticuleAleatoire(gen));
            }
            // Parcours
            double sum = 0.0;
            for (const auto& p : vec) {
                sum += p.getPosition().norme();
            }
            (void)sum;
        }
        auto t1 = std::chrono::steady_clock::now();
        double dt_vec = std::chrono::duration<double, std::milli>(t1 - t0).count();

        // std::list
        t0 = std::chrono::steady_clock::now();
        {
            std::list<Particule> lst;
            for (int i = 0; i < N; ++i) {
                lst.push_back(creerParticuleAleatoire(gen));
            }
            double sum = 0.0;
            for (const auto& p : lst) {
                sum += p.getPosition().norme();
            }
            (void)sum;
        }
        t1 = std::chrono::steady_clock::now();
        double dt_list = std::chrono::duration<double, std::milli>(t1 - t0).count();

        // std::deque
        t0 = std::chrono::steady_clock::now();
        {
            std::deque<Particule> dq;
            for (int i = 0; i < N; ++i) {
                dq.push_back(creerParticuleAleatoire(gen));
            }
            double sum = 0.0;
            for (const auto& p : dq) {
                sum += p.getPosition().norme();
            }
            (void)sum;
        }
        t1 = std::chrono::steady_clock::now();
        double dt_deque = std::chrono::duration<double, std::milli>(t1 - t0).count();

        std::cout << std::setw(10) << N
                  << std::setw(15) << std::fixed << std::setprecision(3) << dt_vec
                  << std::setw(15) << dt_list
                  << std::setw(15) << dt_deque << std::endl;
    }

    // =========================================================================
    // Test de performance du calcul des interactions
    // =========================================================================
    std::cout << "\n=== Performance du calcul des interactions ===" << std::endl;
    std::cout << std::setw(10) << "N"
              << std::setw(20) << "Temps(s)" << std::endl;
    std::cout << std::string(30, '-') << std::endl;

    for (int k = 3; k <= 5; ++k) {
        int N = 1;
        for (int d = 0; d < k; ++d) N *= 2;
        // N^3 particules dans le cube [0,1]³

        Univers univers(3, {1.0, 1.0, 1.0}, 0.0, TypeForce::GRAVITATIONNELLE);

        double spacing = 1.0 / N;
        for (int ix = 0; ix < N; ++ix) {
            for (int iy = 0; iy < N; ++iy) {
                for (int iz = 0; iz < N; ++iz) {
                    Vecteur pos(ix * spacing, iy * spacing, iz * spacing);
                    univers.ajouterParticule(Particule(pos, Vecteur(), 1.0));
                }
            }
        }

        auto t0 = std::chrono::steady_clock::now();
        univers.calculerForces();
        auto t1 = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(t1 - t0).count();

        std::cout << std::setw(10) << (N * N * N)
                  << std::setw(20) << std::fixed << std::setprecision(4) << elapsed
                  << std::endl;
    }

    std::cout << "\nNote : l'optimisation F_ij = -F_ji est déjà active (3e loi de Newton)."
              << std::endl;

    return 0;
}
