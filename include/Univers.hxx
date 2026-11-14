#ifndef UNIVERS_HXX
#define UNIVERS_HXX

/**
 * @file Univers.hxx
 * @brief Déclaration de la classe Univers pour la simulation de particules.
 *
 * Gère la collection de particules, le calcul des forces, l'intégration
 * temporelle (Störmer-Verlet), le découpage spatial, les conditions aux
 * limites et la sortie VTK.
 */

#include "Particule.hxx"
#include "Cellule.hxx"
#include "Vecteur.hxx"
#include <vector>
#include <string>
#include <functional>
#include <stdexcept>

/** @brief Types de forces d'interaction. */
enum class TypeForce {
    GRAVITATIONNELLE,
    LENNARD_JONES
};

/** @brief Types de conditions aux limites. */
enum class ConditionLimite {
    AUCUNE,
    REFLEXION,
    ABSORPTION,
    PERIODIQUE
};

/**
 * @class Univers
 * @brief Univers de simulation avec particules, forces et grille spatiale.
 *
 * Gere le calcul des forces (gravitation ou Lennard-Jones), l'integration
 * Stormer-Verlet, le decoupage spatial, les conditions aux limites
 * et la sortie VTK.
 */
class Univers {
private:
    int m_dimension;                      // 1, 2 ou 3
    std::vector<double> m_longueurs;      // taille du domaine par dimension
    double m_rcut;
    double m_epsilon;                     // parametres LJ
    double m_sigma;
    double m_G;                           // champ gravitationnel
    double m_EcD;                         // energie cinetique cible
    int m_rescaleInterval;

    std::vector<Particule> m_particules;
    std::vector<Cellule>   m_cellules;
    std::vector<int>       m_nbCellules;  // nb cellules par dimension

    TypeForce m_typeForce;
    ConditionLimite m_condLimite;
    bool m_useReflexionPotentiel;

    int m_iteration;

public:
    /// @name Constructeurs
    /// @{

    /** @brief Constructeur par défaut. */
    Univers();

    /** @brief Constructeur paramétré.
     *  @param dimension Dimension (1, 2 ou 3)
     *  @param longueurs Longueurs caractéristiques
     *  @param rcut Rayon de coupure
     *  @param typeForce Type de force d'interaction
     */
    Univers(int dimension, const std::vector<double>& longueurs,
            double rcut, TypeForce typeForce = TypeForce::GRAVITATIONNELLE);

    // configuration
    void setLennardJonesParams(double epsilon, double sigma);

    /** @brief Configure le champ gravitationnel.
     *  @param G Intensité du champ (négatif vers le bas)
     */
    void setGravite(double G);

    /** @brief Configure l'énergie cinétique cible.
     *  @param EcD   Énergie cinétique désirée
     *  @param interval Intervalle de rescaling (nombre d'itérations)
     */
    void setRescaleEnergie(double EcD, int interval = 1000);

    /** @brief Configure la condition aux limites.
     *  @param cl Type de condition aux limites
     *  @param usePotentiel Utiliser le potentiel pour la réflexion
     */
    void setConditionLimite(ConditionLimite cl, bool usePotentiel = false);

    // particules
    void ajouterParticule(const Particule& p);

    /** @brief Retourne le nombre de particules. */
    int getNbParticules() const;

    /** @brief Accès en lecture à une particule.
     *  @param i Indice de la particule
     */
    const Particule& getParticule(int i) const;

    /** @brief Retourne la collection de particules. */
    const std::vector<Particule>& getParticules() const;

    // grille
    void initialiserGrille();

    /** @brief Assigne chaque particule à sa cellule. */
    void assignerParticulesCellules();

    /** @brief Met à jour les cellules après déplacement. */
    void mettreAJourCellules();

    // forces
    void calculerForces();

    /** @brief Calcule les forces avec optimisation par grille. */
    void calculerForcesGrille();

    /** @brief Applique les forces aux parois (réflexion par potentiel). */
    void calculerForcesParois();

    /** @brief Applique le champ gravitationnel externe. */
    void appliquerGravite();

    /**
     * @brief Lance la simulation Stormer-Verlet.
     * @param dt   pas de temps
     * @param tend temps final
     * @param outputPrefix prefixe des fichiers VTK
     * @param outputInterval intervalle de sortie (en iterations)
     */
    void simuler(double dt, double tend,
                 const std::string& outputPrefix = "output",
                 int outputInterval = 100);

    /// @}

    /// @name Conditions aux limites
    /// @{

    /** @brief Applique les conditions aux limites à toutes les particules. */
    void appliquerConditionsLimites();

    // energie
    double energieCinetique() const;

    /** @brief Rescale les vitesses pour atteindre l'énergie cible. */
    void rescaleVitesses();

    // sortie fichiers
    void ecrireVTK(const std::string& filename) const;

    /** @brief Écrit l'état courant dans un fichier texte.
     *  @param filename Nom du fichier de sortie
     *  @param append   Ajouter au fichier existant
     */
    void ecrireTexte(const std::string& filename, bool append = true) const;

    /** @brief Affiche l'état de l'univers. */
    void afficher() const;

private:
    /// @name Méthodes internes
    /// @{

    /** @brief Calcule la force gravitationnelle entre deux particules. */
    Vecteur forceGravitationnelle(const Particule& pi, const Particule& pj) const;

    /** @brief Calcule la force de Lennard-Jones entre deux particules. */
    Vecteur forceLennardJones(const Particule& pi, const Particule& pj) const;

    /** @brief Calcule la force entre deux particules selon le type configuré. */
    Vecteur calculerForceInteraction(const Particule& pi, const Particule& pj) const;

    /** @brief Convertit des coordonnées 3D de cellule en indice linéaire. */
    int celluleIndex(int ix, int iy, int iz) const;

    /** @brief Détermine la cellule d'une particule selon sa position. */
    int trouverCellule(const Vecteur& position) const;
};

#endif // UNIVERS_HXX
