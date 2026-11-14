/**
 * @file Vecteur.cxx
 * @brief Implémentation de la classe Vecteur.
 */

#include "Vecteur.hxx"
#include <stdexcept>

// --- Constructeurs ---

Vecteur::Vecteur() : m_x(0.0), m_y(0.0), m_z(0.0) {}

Vecteur::Vecteur(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}

Vecteur::Vecteur(double x, double y) : m_x(x), m_y(y), m_z(0.0) {}

Vecteur::Vecteur(const Vecteur& other)
    : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {}

// --- Accesseurs ---

double Vecteur::getX() const { return m_x; }
double Vecteur::getY() const { return m_y; }
double Vecteur::getZ() const { return m_z; }

void Vecteur::setX(double x) { m_x = x; }
void Vecteur::setY(double y) { m_y = y; }
void Vecteur::setZ(double z) { m_z = z; }

// --- Operateurs ---

Vecteur& Vecteur::operator=(const Vecteur& other) {
    if (this != &other) {
        m_x = other.m_x;
        m_y = other.m_y;
        m_z = other.m_z;
    }
    return *this;
}

Vecteur& Vecteur::operator+=(const Vecteur& other) {
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
    return *this;
}

Vecteur& Vecteur::operator-=(const Vecteur& other) {
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;
    return *this;
}

Vecteur& Vecteur::operator*=(double scalar) {
    m_x *= scalar;
    m_y *= scalar;
    m_z *= scalar;
    return *this;
}

Vecteur& Vecteur::operator/=(double scalar) {
    if (scalar == 0.0)
        throw std::runtime_error("division par zero");
    m_x /= scalar;
    m_y /= scalar;
    m_z /= scalar;
    return *this;
}

// Opérations vectorielles

double Vecteur::norme() const {
    return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

double Vecteur::normeCarree() const {
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

double Vecteur::dot(const Vecteur& other) const {
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

Vecteur Vecteur::cross(const Vecteur& other) const {
    return Vecteur(
        m_y * other.m_z - m_z * other.m_y,
        m_z * other.m_x - m_x * other.m_z,
        m_x * other.m_y - m_y * other.m_x
    );
}

// --- Fonctions libres ---

Vecteur operator+(const Vecteur& a, const Vecteur& b) {
    return Vecteur(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}

Vecteur operator-(const Vecteur& a, const Vecteur& b) {
    return Vecteur(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
}

Vecteur operator*(const Vecteur& v, double scalar) {
    return Vecteur(v.getX() * scalar, v.getY() * scalar, v.getZ() * scalar);
}

Vecteur operator*(double scalar, const Vecteur& v) {
    return v * scalar;
}

Vecteur operator/(const Vecteur& v, double scalar) {
    if (scalar == 0.0)
        throw std::runtime_error("division par zero");
    return Vecteur(v.getX() / scalar, v.getY() / scalar, v.getZ() / scalar);
}

Vecteur operator-(const Vecteur& v) {
    return Vecteur(-v.getX(), -v.getY(), -v.getZ());
}

bool operator==(const Vecteur& a, const Vecteur& b) {
    const double eps = 1e-12;
    return std::abs(a.getX() - b.getX()) < eps &&
           std::abs(a.getY() - b.getY()) < eps &&
           std::abs(a.getZ() - b.getZ()) < eps;
}

bool operator!=(const Vecteur& a, const Vecteur& b) {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& os, const Vecteur& v) {
    os << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
    return os;
}
