#ifndef PARTICULE_HXX
#define PARTICULE_HXX

/**
 * @file Particule.hxx
 * @brief Déclaration de la classe Particule.
 *
 * Représente une particule tridimensionnelle caractérisée par une
 * position, une vitesse, une masse, un identifiant, une catégorie
 * et une force.
 */

#include "Vecteur.hxx"
#include <string>

/**
 * @class Particule
 * @brief Représente une particule dans une simulation physique.
 *
 * Encapsule les proprietes physiques. L'identifiant est
 * auto-incremente via un compteur statique.
 */
class Particule {
private:
    Vecteur m_position;   ///< Position de la particule
    Vecteur m_vitesse;    ///< Vitesse de la particule
    Vecteur m_force;      ///< Force appliquée à la particule
    Vecteur m_forceOld;   ///< Force de l'itération précédente (Störmer-Verlet)
    double  m_masse;      ///< Masse de la particule
    int     m_id;         ///< Identifiant unique
    int     m_categorie;  ///< Catégorie (type) de particule
    int     m_cellule;    ///< Indice de la cellule contenant la particule

    static int s_nextId;

public:
    /// @name Constructeurs et destructeur
    /// @{

    /** @brief Constructeur par défaut. */
    Particule();

    /** @brief Constructeur complet.
     *  @param position Position initiale
     *  @param vitesse  Vitesse initiale
     *  @param masse    Masse de la particule
     *  @param categorie Catégorie de la particule
     */
    Particule(const Vecteur& position, const Vecteur& vitesse,
              double masse, int categorie = 0);

    /** @brief Constructeur de copie. */
    Particule(const Particule& other);

    /** @brief Destructeur. */
    ~Particule() = default;

    // accesseurs
    const Vecteur& getPosition() const;
    const Vecteur& getVitesse() const;
    const Vecteur& getForce() const;
    const Vecteur& getForceOld() const;
    double getMasse() const;
    int getId() const;
    int getCategorie() const;
    int getCellule() const;

    void setPosition(const Vecteur& pos);
    void setVitesse(const Vecteur& vit);
    void setForce(const Vecteur& f);
    void setForceOld(const Vecteur& f);
    void setCellule(int c);

    /** @brief Ajoute une force a la force courante. 
     *  @param f Force à ajouter
     */
    void ajouterForce(const Vecteur& f);

    /** @brief Remet la force a zero. */
    void resetForce();

    /** @brief Sauvegarde la force courante dans forceOld. */
    void sauvegarderForce();

    /** @brief Met à jour la position selon Störmer-Verlet.
     *  @param dt Pas de temps
     */
    void updatePosition(double dt);

    /** @brief Met à jour la vitesse selon Störmer-Verlet.
     *  @param dt Pas de temps
     */
    void updateVitesse(double dt);

    /** @brief Calcule l'énergie cinétique de la particule. */
    double energieCinetique() const;

    /// @}

    /// @name Opérateurs
    /// @{

    Particule& operator=(const Particule& other);

    /// @}

    /** @brief Opérateur de flux de sortie. */
    friend std::ostream& operator<<(std::ostream& os, const Particule& p);
};

#endif // PARTICULE_HXX
