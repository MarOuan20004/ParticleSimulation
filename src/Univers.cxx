/**
 * @file Univers.cxx
 * @brief Implémentation de la classe Univers.
 *
 * Gère la simulation de particules : forces gravitationnelles et
 * Lennard-Jones, intégration Störmer-Verlet, découpage spatial,
 * conditions aux limites et sortie VTK/texte.
 */

#include "Univers.hxx"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cassert>

// --- Constructeurs ---

Univers::Univers()
    : m_dimension(3), m_rcut(1.0), m_epsilon(1.0), m_sigma(1.0),
      m_G(0.0), m_EcD(0.0), m_rescaleInterval(0),
      m_typeForce(TypeForce::GRAVITATIONNELLE),
      m_condLimite(ConditionLimite::AUCUNE),
      m_useReflexionPotentiel(false), m_iteration(0) {
    m_longueurs = {1.0, 1.0, 1.0};
}

Univers::Univers(int dimension, const std::vector<double>& longueurs,
                 double rcut, TypeForce typeForce)
    : m_dimension(dimension), m_longueurs(longueurs), m_rcut(rcut),
      m_epsilon(1.0), m_sigma(1.0), m_G(0.0), m_EcD(0.0),
      m_rescaleInterval(0), m_typeForce(typeForce),
      m_condLimite(ConditionLimite::AUCUNE),
      m_useReflexionPotentiel(false), m_iteration(0) {
    if (dimension < 1 || dimension > 3) {
        throw std::invalid_argument("Univers : dimension doit être 1, 2 ou 3");
    }
    if (longueurs.size() < static_cast<size_t>(dimension)) {
        throw std::invalid_argument("Univers : pas assez de longueurs caractéristiques");
    }
}

// --- Configuration ---

void Univers::setLennardJonesParams(double epsilon, double sigma) {
    m_epsilon = epsilon;
    m_sigma = sigma;
}

void Univers::setGravite(double G) {
    m_G = G;
}

void Univers::setRescaleEnergie(double EcD, int interval) {
    m_EcD = EcD;
    m_rescaleInterval = interval;
}

void Univers::setConditionLimite(ConditionLimite cl, bool usePotentiel) {
    m_condLimite = cl;
    m_useReflexionPotentiel = usePotentiel;
}

// --- Particules ---

void Univers::ajouterParticule(const Particule& p) {
    m_particules.push_back(p);
}

int Univers::getNbParticules() const {
    return static_cast<int>(m_particules.size());
}

const Particule& Univers::getParticule(int i) const {
    if (i < 0 || i >= static_cast<int>(m_particules.size())) {
        throw std::out_of_range("Univers::getParticule : indice hors limites");
    }
    return m_particules[i];
}

const std::vector<Particule>& Univers::getParticules() const {
    return m_particules;
}

// --- Grille spatiale ---

void Univers::initialiserGrille() {
    if (m_rcut <= 0.0) return; // Pas de grille si pas de rayon de coupure

    m_nbCellules.resize(3, 1);
    for (int d = 0; d < m_dimension; ++d) {
        m_nbCellules[d] = std::max(1, static_cast<int>(std::floor(m_longueurs[d] / m_rcut)));
    }

    int totalCellules = m_nbCellules[0] * m_nbCellules[1] * m_nbCellules[2];
    m_cellules.clear();
    m_cellules.reserve(totalCellules);
    for (int i = 0; i < totalCellules; ++i) {
        m_cellules.emplace_back(i);
    }

    // Construction du voisinage
    for (int iz = 0; iz < m_nbCellules[2]; ++iz) {
        for (int iy = 0; iy < m_nbCellules[1]; ++iy) {
            for (int ix = 0; ix < m_nbCellules[0]; ++ix) {
                int idx = celluleIndex(ix, iy, iz);
                // Parcours des 27 (3D), 9 (2D) ou 3 (1D) cellules voisines
                for (int dz = -1; dz <= 1; ++dz) {
                    if (m_dimension < 3 && dz != 0) continue;
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (m_dimension < 2 && dy != 0) continue;
                        for (int dx = -1; dx <= 1; ++dx) {
                            int nx = ix + dx;
                            int ny = iy + dy;
                            int nz = iz + dz;

                            // Conditions périodiques pour le voisinage
                            if (m_condLimite == ConditionLimite::PERIODIQUE) {
                                nx = (nx + m_nbCellules[0]) % m_nbCellules[0];
                                ny = (ny + m_nbCellules[1]) % m_nbCellules[1];
                                nz = (nz + m_nbCellules[2]) % m_nbCellules[2];
                            } else {
                                if (nx < 0 || nx >= m_nbCellules[0]) continue;
                                if (ny < 0 || ny >= m_nbCellules[1]) continue;
                                if (nz < 0 || nz >= m_nbCellules[2]) continue;
                            }

                            int nidx = celluleIndex(nx, ny, nz);
                            if (nidx != idx) {
                                m_cellules[idx].ajouterVoisine(nidx);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Univers::assignerParticulesCellules() {
    for (auto& c : m_cellules) {
        c.vider();
    }
    for (int i = 0; i < static_cast<int>(m_particules.size()); ++i) {
        int ci = trouverCellule(m_particules[i].getPosition());
        if (ci >= 0 && ci < static_cast<int>(m_cellules.size())) {
            m_cellules[ci].ajouterParticule(i);
            m_particules[i].setCellule(ci);
        }
    }
}

void Univers::mettreAJourCellules() {
    assignerParticulesCellules();
}

int Univers::celluleIndex(int ix, int iy, int iz) const {
    return ix + iy * m_nbCellules[0] + iz * m_nbCellules[0] * m_nbCellules[1];
}

int Univers::trouverCellule(const Vecteur& position) const {
    if (m_cellules.empty()) return -1;

    int ix = static_cast<int>(std::floor(position.getX() / m_rcut));
    int iy = static_cast<int>(std::floor(position.getY() / m_rcut));
    int iz = static_cast<int>(std::floor(position.getZ() / m_rcut));

    // Clamp aux bornes
    ix = std::max(0, std::min(ix, m_nbCellules[0] - 1));
    iy = std::max(0, std::min(iy, m_nbCellules[1] - 1));
    iz = std::max(0, std::min(iz, m_nbCellules[2] - 1));

    return celluleIndex(ix, iy, iz);
}

// --- Calcul des forces ---

Vecteur Univers::forceGravitationnelle(const Particule& pi,
                                        const Particule& pj) const {
    Vecteur rij = pj.getPosition() - pi.getPosition();
    double dist = rij.norme();
    if (dist < 1e-10) return Vecteur(); // Éviter division par zéro
    return rij * (pi.getMasse() * pj.getMasse() / (dist * dist * dist));
}

Vecteur Univers::forceLennardJones(const Particule& pi,
                                    const Particule& pj) const {
    Vecteur rij = pj.getPosition() - pi.getPosition();
    double r2 = rij.normeCarree();
    if (r2 < 1e-20) return Vecteur(); // Éviter division par zéro

    double r = std::sqrt(r2);

    // Vérifier le rayon de coupure
    if (m_rcut > 0 && r > m_rcut) return Vecteur();

    double sr6 = std::pow(m_sigma / r, 6);
    // F_ij = 24*eps/r^2 * (sigma/r)^6 * (1 - 2*(sigma/r)^6) * r_ij
    double coeff = 24.0 * m_epsilon / r2 * sr6 * (1.0 - 2.0 * sr6);
    return rij * coeff;
}

Vecteur Univers::calculerForceInteraction(const Particule& pi,
                                           const Particule& pj) const {
    switch (m_typeForce) {
        case TypeForce::GRAVITATIONNELLE:
            return forceGravitationnelle(pi, pj);
        case TypeForce::LENNARD_JONES:
            return forceLennardJones(pi, pj);
        default:
            return Vecteur();
    }
}

void Univers::calculerForces() {
    int n = static_cast<int>(m_particules.size());

    // Reset des forces
    for (auto& p : m_particules) {
        p.resetForce();
    }

    // Optimisation : utilisation de la 3e loi de Newton (F_ij = -F_ji)
    // Divise le temps de calcul par ~2
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            Vecteur fij = calculerForceInteraction(m_particules[i], m_particules[j]);
            m_particules[i].ajouterForce(fij);
            m_particules[j].ajouterForce(-fij);
        }
    }

    // Ajouter le champ gravitationnel externe
    appliquerGravite();

    // Forces des parois si réflexion par potentiel
    if (m_condLimite == ConditionLimite::REFLEXION && m_useReflexionPotentiel) {
        calculerForcesParois();
    }
}

void Univers::calculerForcesGrille() {
    // Reset des forces
    for (auto& p : m_particules) {
        p.resetForce();
    }

    // Parcours des cellules
    for (auto& cellule : m_cellules) {
        const auto& indices = cellule.getParticules();

        // Interactions intra-cellule
        for (size_t a = 0; a < indices.size(); ++a) {
            for (size_t b = a + 1; b < indices.size(); ++b) {
                int i = indices[a];
                int j = indices[b];
                Vecteur fij = calculerForceInteraction(m_particules[i], m_particules[j]);
                m_particules[i].ajouterForce(fij);
                m_particules[j].ajouterForce(-fij);
            }
        }

        // Interactions avec les cellules voisines
        for (int voisIdx : cellule.getVoisines()) {
            // Pour éviter le double comptage, ne traiter que si voisIdx > cellule.getIndex()
            if (voisIdx <= cellule.getIndex()) continue;

            const auto& voisParticules = m_cellules[voisIdx].getParticules();
            for (int i : indices) {
                for (int j : voisParticules) {
                    Vecteur fij = calculerForceInteraction(m_particules[i], m_particules[j]);
                    m_particules[i].ajouterForce(fij);
                    m_particules[j].ajouterForce(-fij);
                }
            }
        }
    }

    // Ajouter le champ gravitationnel externe
    appliquerGravite();

    // Forces des parois si réflexion par potentiel
    if (m_condLimite == ConditionLimite::REFLEXION && m_useReflexionPotentiel) {
        calculerForcesParois();
    }
}

void Univers::calculerForcesParois() {
    double rcutMur = std::pow(2.0, 1.0 / 6.0) * m_sigma;

    for (auto& p : m_particules) {
        Vecteur pos = p.getPosition();
        Vecteur fParoi;

        for (int d = 0; d < m_dimension; ++d) {
            double coord = (d == 0) ? pos.getX() : (d == 1) ? pos.getY() : pos.getZ();
            double Ld = m_longueurs[d];

            // Distance au bord inférieur
            double rInf = coord;
            if (rInf < rcutMur && rInf > 1e-10) {
                double sr6 = std::pow(m_sigma / (2.0 * rInf), 6);
                double fMag = -24.0 * m_epsilon / (2.0 * rInf) * sr6 * (1.0 - 2.0 * sr6);
                // Force dirigée vers l'intérieur (direction positive)
                if (d == 0)      fParoi.setX(fParoi.getX() - fMag);
                else if (d == 1) fParoi.setY(fParoi.getY() - fMag);
                else             fParoi.setZ(fParoi.getZ() - fMag);
            }

            // Distance au bord supérieur
            double rSup = Ld - coord;
            if (rSup < rcutMur && rSup > 1e-10) {
                double sr6 = std::pow(m_sigma / (2.0 * rSup), 6);
                double fMag = -24.0 * m_epsilon / (2.0 * rSup) * sr6 * (1.0 - 2.0 * sr6);
                // Force dirigée vers l'intérieur (direction négative)
                if (d == 0)      fParoi.setX(fParoi.getX() + fMag);
                else if (d == 1) fParoi.setY(fParoi.getY() + fMag);
                else             fParoi.setZ(fParoi.getZ() + fMag);
            }
        }

        p.ajouterForce(fParoi);
    }
}

void Univers::appliquerGravite() {
    if (std::abs(m_G) < 1e-15) return;
    for (auto& p : m_particules) {
        // Force gravitationnelle = (0, m_i * G, 0) — direction verticale y
        p.ajouterForce(Vecteur(0.0, p.getMasse() * m_G, 0.0));
    }
}

// --- Conditions aux limites ---

void Univers::appliquerConditionsLimites() {
    // Note : avec la réflexion par potentiel, les forces des parois
    // sont déjà appliquées dans calculerForces(), pas besoin de réflexion cinématique
    if (m_condLimite == ConditionLimite::REFLEXION && m_useReflexionPotentiel) {
        return;
    }

    auto it = m_particules.begin();
    while (it != m_particules.end()) {
        bool supprime = false;
        Vecteur pos = it->getPosition();
        Vecteur vit = it->getVitesse();

        for (int d = 0; d < m_dimension; ++d) {
            double coord = (d == 0) ? pos.getX() : (d == 1) ? pos.getY() : pos.getZ();
            double Ld = m_longueurs[d];

            if (coord < 0 || coord > Ld) {
                switch (m_condLimite) {
                    case ConditionLimite::REFLEXION: {
                        // Inversion de la composante de vitesse
                        if (coord < 0) {
                            if (d == 0) { pos.setX(-coord); vit.setX(-vit.getX()); }
                            else if (d == 1) { pos.setY(-coord); vit.setY(-vit.getY()); }
                            else { pos.setZ(-coord); vit.setZ(-vit.getZ()); }
                        } else {
                            if (d == 0) { pos.setX(2.0 * Ld - coord); vit.setX(-vit.getX()); }
                            else if (d == 1) { pos.setY(2.0 * Ld - coord); vit.setY(-vit.getY()); }
                            else { pos.setZ(2.0 * Ld - coord); vit.setZ(-vit.getZ()); }
                        }
                        break;
                    }
                    case ConditionLimite::ABSORPTION:
                        supprime = true;
                        break;
                    case ConditionLimite::PERIODIQUE: {
                        // Transport sur la paroi opposée
                        double newCoord = std::fmod(coord, Ld);
                        if (newCoord < 0) newCoord += Ld;
                        if (d == 0) pos.setX(newCoord);
                        else if (d == 1) pos.setY(newCoord);
                        else pos.setZ(newCoord);
                        break;
                    }
                    default:
                        break;
                }
            }

            if (supprime) break;
        }

        if (supprime) {
            it = m_particules.erase(it);
        } else {
            it->setPosition(pos);
            it->setVitesse(vit);
            ++it;
        }
    }
}

// --- Energie ---

double Univers::energieCinetique() const {
    double ec = 0.0;
    for (const auto& p : m_particules) {
        ec += p.energieCinetique();
    }
    return ec;
}

void Univers::rescaleVitesses() {
    if (m_EcD <= 0.0) return;
    double ec = energieCinetique();
    if (ec < 1e-20) return;
    double beta = std::sqrt(m_EcD / ec);
    for (auto& p : m_particules) {
        p.setVitesse(p.getVitesse() * beta);
    }
}

// --- Boucle de simulation ---

void Univers::simuler(double dt, double tend,
                      const std::string& outputPrefix, int outputInterval) {
    bool useGrille = !m_cellules.empty();

    // Initialisation : calcul des forces
    if (useGrille) {
        assignerParticulesCellules();
        calculerForcesGrille();
    } else {
        calculerForces();
    }

    double t = 0.0;
    m_iteration = 0;

    // Sortie initiale
    {
        std::ostringstream oss;
        oss << outputPrefix << "_" << std::setw(6) << std::setfill('0') << m_iteration << ".vtu";
        ecrireVTK(oss.str());
    }

    while (t < tend) {
        t += dt;
        m_iteration++;

        // Étape 1 : mise à jour des positions et sauvegarde des forces
        for (auto& p : m_particules) {
            p.updatePosition(dt);
            p.sauvegarderForce();
        }

        // Appliquer les conditions aux limites
        appliquerConditionsLimites();

        // Étape 2 : recalcul des forces
        if (useGrille) {
            mettreAJourCellules();
            calculerForcesGrille();
        } else {
            calculerForces();
        }

        // Étape 3 : mise à jour des vitesses
        for (auto& p : m_particules) {
            p.updateVitesse(dt);
        }

        // Rescaling de l'énergie cinétique si configuré
        if (m_rescaleInterval > 0 && m_iteration % m_rescaleInterval == 0) {
            rescaleVitesses();
        }

        // Sortie périodique
        if (m_iteration % outputInterval == 0) {
            std::ostringstream oss;
            oss << outputPrefix << "_" << std::setw(6) << std::setfill('0') << m_iteration << ".vtu";
            ecrireVTK(oss.str());
            std::cout << "t = " << std::fixed << std::setprecision(4) << t
                      << "  |  N = " << m_particules.size()
                      << "  |  Ec = " << std::scientific << energieCinetique()
                      << std::endl;
        }
    }
}

// --- Sortie VTK ---

void Univers::ecrireVTK(const std::string& filename) const {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filename);
    }

    int n = static_cast<int>(m_particules.size());

    ofs << std::fixed << std::setprecision(6);
    ofs << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    ofs << "  <UnstructuredGrid>\n";
    ofs << "    <Piece NumberOfPoints=\"" << n << "\" NumberOfCells=\"0\">\n";

    // Positions
    ofs << "      <Points>\n";
    ofs << "        <DataArray name=\"Position\" type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    ofs << "          ";
    for (int i = 0; i < n; ++i) {
        const Vecteur& pos = m_particules[i].getPosition();
        ofs << pos.getX() << " " << pos.getY() << " " << pos.getZ() << " ";
        if (i % 10 == 9) ofs << "\n          ";
    }
    ofs << "\n";
    ofs << "        </DataArray>\n";
    ofs << "      </Points>\n";

    // Donnees des points
    ofs << "      <PointData Vectors=\"Velocity\">\n";

    // Vitesses
    ofs << "        <DataArray type=\"Float32\" Name=\"Velocity\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    ofs << "          ";
    for (int i = 0; i < n; ++i) {
        const Vecteur& vit = m_particules[i].getVitesse();
        ofs << vit.getX() << " " << vit.getY() << " " << vit.getZ() << " ";
        if (i % 10 == 9) ofs << "\n          ";
    }
    ofs << "\n";
    ofs << "        </DataArray>\n";

    // Masses
    ofs << "        <DataArray type=\"Float32\" Name=\"Masse\" format=\"ascii\">\n";
    ofs << "          ";
    for (int i = 0; i < n; ++i) {
        ofs << m_particules[i].getMasse() << " ";
        if (i % 20 == 19) ofs << "\n          ";
    }
    ofs << "\n";
    ofs << "        </DataArray>\n";

    ofs << "      </PointData>\n";

    // Cellules (vides pour des données de particules)
    ofs << "      <Cells>\n";
    ofs << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n";
    ofs << "        </DataArray>\n";
    ofs << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n";
    ofs << "        </DataArray>\n";
    ofs << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
    ofs << "        </DataArray>\n";
    ofs << "      </Cells>\n";

    ofs << "    </Piece>\n";
    ofs << "  </UnstructuredGrid>\n";
    ofs << "</VTKFile>\n";

    ofs.close();
}

void Univers::ecrireTexte(const std::string& filename, bool append) const {
    std::ofstream ofs;
    if (append) {
        ofs.open(filename, std::ios::app);
    } else {
        ofs.open(filename);
    }

    if (!ofs.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filename);
    }

    for (const auto& p : m_particules) {
        ofs << p.getPosition().getX() << " " << p.getPosition().getY() << " ";
    }
    ofs << "\n";
    ofs.close();
}

void Univers::afficher() const {
    std::cout << "=== Univers [dim=" << m_dimension
              << ", N=" << m_particules.size() << "] ===" << std::endl;
    for (const auto& p : m_particules) {
        std::cout << "  " << p << std::endl;
    }
}
