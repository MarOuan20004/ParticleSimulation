#ifndef VECTEUR_HXX
#define VECTEUR_HXX

/**
 * @file Vecteur.hxx
 * @brief Déclaration de la classe Vecteur pour les vecteurs 3D.
 *
 * Cette classe encapsule un vecteur de dimension 3 avec tous les
 * opérateurs arithmétiques et algébriques nécessaires à la simulation
 * de particules.
 */

#include <iostream>
#include <cmath>

/**
 * @class Vecteur
 * @brief Represente un vecteur tridimensionnel.
 *
 * Fournit les operations arithmetiques (+, -, *, /), le produit scalaire,
 * le produit vectoriel, la norme, et les operateurs de comparaison.
 */
class Vecteur {
private:
    double m_x;
    double m_y;
    double m_z;

public:
    // constructeurs
    Vecteur();

    /** @brief Constructeur avec composantes.
     *  @param x Composante x
     *  @param y Composante y
     *  @param z Composante z
     */
    Vecteur(double x, double y, double z);

    /** @brief Constructeur 2D (z=0).
     *  @param x Composante x
     *  @param y Composante y
     */
    Vecteur(double x, double y);

    /** @brief Constructeur de copie. */
    Vecteur(const Vecteur& other);

    /** @brief Destructeur. */
    ~Vecteur() = default;

    // accesseurs
    /** @brief Retourne la composante x. */
    double getX() const;
    /** @brief Retourne la composante y. */
    double getY() const;
    /** @brief Retourne la composante z. */
    double getZ() const;

    /** @brief Modifie la composante x. */
    void setX(double x);
    /** @brief Modifie la composante y. */
    void setY(double y);
    /** @brief Modifie la composante z. */
    void setZ(double z);

    // operateurs membres
    Vecteur& operator=(const Vecteur& other);
    Vecteur& operator+=(const Vecteur& other);
    Vecteur& operator-=(const Vecteur& other);
    Vecteur& operator*=(double scalar);
    Vecteur& operator/=(double scalar);

    /** @brief Calcule la norme euclidienne. */
    double norme() const;

    /** @brief Carre de la norme (evite le sqrt). */
    double normeCarree() const;

    /** @brief Produit scalaire avec un autre vecteur. */
    double dot(const Vecteur& other) const;

    /** @brief Produit vectoriel. */
    Vecteur cross(const Vecteur& other) const;
};

/// @name Opérateurs binaires (fonctions libres)
/// @{

Vecteur operator+(const Vecteur& a, const Vecteur& b);
Vecteur operator-(const Vecteur& a, const Vecteur& b);
Vecteur operator*(const Vecteur& v, double scalar);
Vecteur operator*(double scalar, const Vecteur& v);
Vecteur operator/(const Vecteur& v, double scalar);
Vecteur operator-(const Vecteur& v);

bool operator==(const Vecteur& a, const Vecteur& b);
bool operator!=(const Vecteur& a, const Vecteur& b);

/** @brief Opérateur de flux de sortie. */
std::ostream& operator<<(std::ostream& os, const Vecteur& v);

/// @}

#endif // VECTEUR_HXX
