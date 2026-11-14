// test_boundary.cxx - tests conditions aux limites
#include <gtest/gtest.h>
#include "Univers.hxx"
#include <cmath>

TEST(BoundaryTest, Reflexion) {
    Univers u(2, {10.0, 10.0}, 0.0, TypeForce::GRAVITATIONNELLE);
    u.setConditionLimite(ConditionLimite::REFLEXION);
    // Particule qui sort par la droite
    u.ajouterParticule(Particule(Vecteur(10.5, 5.0), Vecteur(1.0, 0.0), 1.0));
    u.appliquerConditionsLimites();
    // Position réfléchie : 2*10 - 10.5 = 9.5
    EXPECT_NEAR(u.getParticule(0).getPosition().getX(), 9.5, 1e-10);
    // Vitesse inversée
    EXPECT_NEAR(u.getParticule(0).getVitesse().getX(), -1.0, 1e-10);
}

TEST(BoundaryTest, ReflexionBordInferieur) {
    Univers u(2, {10.0, 10.0}, 0.0, TypeForce::GRAVITATIONNELLE);
    u.setConditionLimite(ConditionLimite::REFLEXION);
    // Particule qui sort par le bas
    u.ajouterParticule(Particule(Vecteur(5.0, -0.5), Vecteur(0.0, -1.0), 1.0));
    u.appliquerConditionsLimites();
    EXPECT_NEAR(u.getParticule(0).getPosition().getY(), 0.5, 1e-10);
    EXPECT_NEAR(u.getParticule(0).getVitesse().getY(), 1.0, 1e-10);
}

TEST(BoundaryTest, Absorption) {
    Univers u(2, {10.0, 10.0}, 0.0, TypeForce::GRAVITATIONNELLE);
    u.setConditionLimite(ConditionLimite::ABSORPTION);
    u.ajouterParticule(Particule(Vecteur(5.0, 5.0), Vecteur(), 1.0));
    u.ajouterParticule(Particule(Vecteur(11.0, 5.0), Vecteur(), 1.0)); // hors limites
    u.appliquerConditionsLimites();
    EXPECT_EQ(u.getNbParticules(), 1);
}

TEST(BoundaryTest, Periodique) {
    Univers u(2, {10.0, 10.0}, 0.0, TypeForce::GRAVITATIONNELLE);
    u.setConditionLimite(ConditionLimite::PERIODIQUE);
    u.ajouterParticule(Particule(Vecteur(11.0, 5.0), Vecteur(1.0, 0.0), 1.0));
    u.appliquerConditionsLimites();
    EXPECT_NEAR(u.getParticule(0).getPosition().getX(), 1.0, 1e-10);
    // La vitesse ne change pas pour les conditions périodiques
    EXPECT_NEAR(u.getParticule(0).getVitesse().getX(), 1.0, 1e-10);
}

TEST(BoundaryTest, PeriodiqueNegatif) {
    Univers u(2, {10.0, 10.0}, 0.0, TypeForce::GRAVITATIONNELLE);
    u.setConditionLimite(ConditionLimite::PERIODIQUE);
    u.ajouterParticule(Particule(Vecteur(-1.0, 5.0), Vecteur(), 1.0));
    u.appliquerConditionsLimites();
    EXPECT_NEAR(u.getParticule(0).getPosition().getX(), 9.0, 1e-10);
}

TEST(BoundaryTest, RescaleVitesses) {
    Univers u;
    u.ajouterParticule(Particule(Vecteur(), Vecteur(10.0, 0.0, 0.0), 1.0));
    // Ec = 0.5 * 1 * 100 = 50
    u.setRescaleEnergie(12.5); // beta = sqrt(12.5/50) = 0.5
    u.rescaleVitesses();
    EXPECT_NEAR(u.getParticule(0).getVitesse().getX(), 5.0, 1e-10);
}
