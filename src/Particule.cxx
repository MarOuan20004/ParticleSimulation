/**
 * @file Particule.cxx
 * @brief Implémentation de la classe Particule.
 */

#include "Particule.hxx"

/// Initialisation du compteur statique d'identifiants
int Particule::s_nextId = 0;

// Constructeurs

Particule::Particule() : 
    m_position(), m_vitesse(), m_force(), m_forceOld(), 
    m_masse(1.0), m_id(s_nextId++), m_categorie(0), m_cellule(-1) 
{}

Particule::Particule(const Vecteur& position, const Vecteur& vitesse, double masse, int categorie) : 
    m_position(position), m_vitesse(vitesse), m_force(), m_forceOld(), 
    m_masse(masse), m_id(s_nextId++), m_categorie(categorie), m_cellule(-1) 
{}

Particule::Particule(const Particule& other)
    : m_position(other.m_position), m_vitesse(other.m_vitesse),
      m_force(other.m_force), m_forceOld(other.m_forceOld),
      m_masse(other.m_masse), m_id(other.m_id),
      m_categorie(other.m_categorie), m_cellule(other.m_cellule) {}

// Accesseurs

const Vecteur& Particule::getPosition() const { return m_position; }
const Vecteur& Particule::getVitesse() const { return m_vitesse; }
const Vecteur& Particule::getForce() const { return m_force; }
const Vecteur& Particule::getForceOld() const { return m_forceOld; }
double Particule::getMasse() const { return m_masse; }
int Particule::getId() const { return m_id; }
int Particule::getCategorie() const { return m_categorie; }
int Particule::getCellule() const { return m_cellule; }

void Particule::setPosition(const Vecteur& pos) { m_position = pos; }
void Particule::setVitesse(const Vecteur& vit) { m_vitesse = vit; }
void Particule::setForce(const Vecteur& f) { m_force = f; }
void Particule::setForceOld(const Vecteur& f) { m_forceOld = f; }
void Particule::setCellule(int c) { m_cellule = c; }

// Méthodes de simulation

void Particule::ajouterForce(const Vecteur& f) {
    m_force += f;
}

void Particule::resetForce() {
    m_force = Vecteur(); // Remise a zero
}

void Particule::sauvegarderForce() {
    m_forceOld = m_force;
}

void Particule::updatePosition(double dt) {
    // Calcul de l'acceleration : a = F/m
    Vecteur acc = m_force * (1.0 / m_masse);
    
    // x = x + dt * v + 0.5 * a * dt^2
    Vecteur deplacement = (m_vitesse + acc * (0.5 * dt)) * dt;
    m_position += deplacement;
}

void Particule::updateVitesse(double dt) {
    // v = v + dt * 0.5 * (a + a_old)
    Vecteur f_moyenne = (m_force + m_forceOld) * 0.5;
    Vecteur acc_moyenne = f_moyenne * (1.0 / m_masse);
    m_vitesse += acc_moyenne * dt;
}

double Particule::energieCinetique() const {
    return 0.5 * m_masse * m_vitesse.normeCarree();
}

// Opérateurs

Particule& Particule::operator=(const Particule& other) {
    if (this != &other) {
        m_position = other.m_position;
        m_vitesse = other.m_vitesse;
        m_force = other.m_force;
        m_forceOld = other.m_forceOld;
        m_masse = other.m_masse;
        m_id = other.m_id;
        m_categorie = other.m_categorie;
        m_cellule = other.m_cellule;
    }
    return *this;
}
std::ostream& operator<<(std::ostream& os, const Particule& p) {
    os << "Particule[id=" << p.m_id
       << ", pos=" << p.m_position
       << ", vit=" << p.m_vitesse
       << ", m=" << p.m_masse
       << ", cat=" << p.m_categorie << "]";
    return os;
}
