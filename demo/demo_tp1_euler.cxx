/**
 * @file demo_tp1_euler.cxx
 * @brief TP1 — Résolution d'EDO : Euler explicite et implicite.
 *
 * Cas test 1 : u'(x)=2*x*u(x), u(0)=1, x∈[0,1]
 *   Solution analytique : u(x) = exp(x²)
 *
 * Cas test 2 (raide) : u'(x)= -50*(u(x) - cos(x)), u(0) = 0, x ∈ [0, 2]
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>
#include <vector>
#include <string>
#include <iomanip>

// --- Fonctions pour les cas tests ---

// Cas 1 : u' = 2xu
double f1(double x, double u) {
    return 2.0*x*u;
}
double sol_exacte1(double x) {
    return std::exp(x*x);
}

// Cas 2 : u' = -50(u - cos(x))
double f2(double x, double u) {
    return -50.0*(u - std::cos(x));
}

// Euler explicite : u_{n+1} = u_n + h * phi(x_n, u_n)
std::vector<std::pair<double, double>> euler_explicite(
    std::function<double(double, double)> phi,
    double a, double b, double ua, int N) {

    double h=(b-a)/N;
    std::vector<std::pair<double, double>> result;
    // on laisse le vector grandir seul

    double x=a;
    double u=ua;
    result.push_back(std::make_pair(x,u));

    for (int n=0; n<N; ++n) {
        // etape par etape
        double pente=phi(x,u);
        double increment=h*pente;
        u=u+increment;
        
        x=x+h;
        result.push_back(std::make_pair(x,u));
    }

    return result;
}

// Euler implicite avec iterations de point fixe
std::vector<std::pair<double, double>> euler_implicite(
    std::function<double(double, double)> phi,
    double a, double b, double ua, int N, int maxIter = 100) {

    double h=(b-a)/N;
    std::vector<std::pair<double, double>> result;
    // pas de reserve ici non plus

    double x=a;
    double u=ua;
    result.push_back(std::make_pair(x,u));

    for (int n=0; n<N; ++n) {
        double x_next=x+h;
        
        // Iterations de point fixe pour trouver u_{n+1}
        // u_next = u + h * phi(x_next, u_next)
        double u_guess=u+h*phi(x,u); // prediction avec euler explicite
        
        for (int i=0; i<maxIter; ++i) {
            double u_old=u_guess;
            double f_val=phi(x_next, u_old);
            u_guess=u+h*f_val;
            
            double erreur=std::abs(u_guess-u_old);
            if (erreur<1e-12) {
                break;
            }
        }
        u=u_guess;
        x=x_next;
        result.push_back(std::make_pair(x,u));
    }

    return result;
}

// ecrit les resultats dans un fichier
void ecrire_resultats(const std::string& filename,
                      const std::vector<std::pair<double, double>>& data,
                      std::function<double(double)> solution_exacte = nullptr) {
    std::ofstream ofs(filename);
    ofs << std::setprecision(10);
    ofs << "# x   u_num   u_exact   erreur\n";
    for (const auto& [x, u] : data) {
        ofs << x << " " << u;
        if (solution_exacte) {
            double exact = solution_exacte(x);
            ofs << " " << exact << " " << std::abs(u - exact);
        }
        ofs << "\n";
    }
    ofs.close();
}

int main() {
    // ---- Cas test 1 ----
    std::cout << "- Cas test 1 : u'(x) = 2*x*u(x) " << std::endl;

    for (int N : {10, 100, 1000, 10000}) {
        auto res_exp = euler_explicite(f1, 0.0, 1.0, 1.0, N);
        auto res_imp = euler_implicite(f1, 0.0, 1.0, 1.0, N);

        double err_exp = std::abs(res_exp.back().second - sol_exacte1(1.0));
        double err_imp = std::abs(res_imp.back().second - sol_exacte1(1.0));

        std::cout << "N = " << N << " \th = " << 1.0/N 
                  << " \terr_exp = " << err_exp 
                  << " \terr_imp = " << err_imp << std::endl;

        if (N == 1000) {
            ecrire_resultats("euler_explicite_cas1.dat", res_exp);
            ecrire_resultats("euler_implicite_cas1.dat", res_imp);
        }
    }

    // ---- Cas test 2 (raide) ----
    std::cout << "\n- Cas test 2 (raide) : u'(x) = -50*(u - cos(x)) " << std::endl;

    for (int N : {100, 1000, 10000, 100000}) {
        auto res_exp = euler_explicite(f2, 0.0, 2.0, 0.0, N);
        auto res_imp = euler_implicite(f2, 0.0, 2.0, 0.0, N);

        std::cout << "N = " << N << " \th = " << 2.0/N 
                  << " \tu_exp(2) = " << res_exp.back().second 
                  << " \tu_imp(2) = " << res_imp.back().second << std::endl;

        if (N == 10000) {
            ecrire_resultats("euler_explicite_raide.dat", res_exp);
            ecrire_resultats("euler_implicite_raide.dat", res_imp);
        }
    }

    std::cout << "\nNote : pour le cas raide, Euler explicite nécessite h < 2/50 = 0.04"
              << " pour la stabilité." << std::endl;

    return 0;
}
