#ifndef CELLULE_HXX
#define CELLULE_HXX

/**
 * @file Cellule.hxx
 * @brief Déclaration de la classe Cellule pour le découpage spatial.
 *
 * Représente une cellule de la grille tensorielle utilisée pour
 * optimiser le calcul des interactions entre particules via un rayon
 * de coupure.
 */

#include <vector>

/**
 * @class Cellule
 * @brief Case de la grille contenant des indices de particules et de voisines.
 */
class Cellule {
private:
    int m_index;
    std::vector<int> m_particules;
    std::vector<int> m_voisines;

public:
    /// @name Constructeurs
    /// @{

    /** @brief Constructeur par défaut. */
    Cellule();

    /** @brief Constructeur avec indice.
     *  @param index Indice global de la cellule
     */
    explicit Cellule(int index);

    int getIndex() const;
    const std::vector<int>& getParticules() const;
    const std::vector<int>& getVoisines() const;

    /// @}

    /// @name Gestion des particules
    /// @{

    /** @brief Ajoute une particule à cette cellule.
     *  @param idx Indice de la particule
     */
    void ajouterParticule(int idx);

    /** @brief Retire une particule de cette cellule.
     *  @param idx Indice de la particule
     */
    void retirerParticule(int idx);

    /** @brief Vide toutes les particules de la cellule. */
    void vider();

    /// @}

    /// @name Gestion du voisinage
    /// @{

    /** @brief Ajoute une cellule voisine.
     *  @param idx Indice de la cellule voisine
     */
    void ajouterVoisine(int idx);
};

#endif // CELLULE_HXX
