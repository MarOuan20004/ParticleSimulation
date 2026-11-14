/**
 * @file Cellule.cxx
 * @brief Implémentation de la classe Cellule.
 */

#include "Cellule.hxx"
#include <algorithm>

// =============================================================================
// Constructeurs
// =============================================================================

Cellule::Cellule() : m_index(-1) {}

Cellule::Cellule(int index) : m_index(index) {}

// =============================================================================
// Accesseurs
// =============================================================================

int Cellule::getIndex() const { return m_index; }

const std::vector<int>& Cellule::getParticules() const {
    return m_particules;
}

const std::vector<int>& Cellule::getVoisines() const {
    return m_voisines;
}

// =============================================================================
// Gestion des particules
// =============================================================================

void Cellule::ajouterParticule(int idx) {
    m_particules.push_back(idx);
}

void Cellule::retirerParticule(int idx) {
    auto it = std::find(m_particules.begin(), m_particules.end(), idx);
    if (it != m_particules.end()) {
        m_particules.erase(it);
    }
}

void Cellule::vider() {
    m_particules.clear();
}

// =============================================================================
// Gestion du voisinage
// =============================================================================

void Cellule::ajouterVoisine(int idx) {
    m_voisines.push_back(idx);
}

