// test_univers.cxx - tests pour Univers
#include <gtest/gtest.h>
#include "Univers.hxx"
#include <cmath>

TEST(UniversTest, ConstructeurDefaut) {
    Univers u;
    EXPECT_EQ(u.getNbParticules(), 0);
}

TEST(UniversTest, ConstructeurParametre) {
    Univers u(2, {10.0, 10.0}, 2.5, TypeForce::LENNARD_JONES);
    EXPECT_EQ(u.getNbParticules(), 0);
}

TEST(UniversTest, DimensionInvalide) {
    EXPECT_THROW(Univers(0, {}, 1.0), std::invalid_argument);
    EXPECT_THROW(Univers(4, {1,1,1,1}, 1.0), std::invalid_argument);
}

TEST(UniversTest, AjoutParticules) {
    Univers u;
    u.ajouterParticule(Particule(Vecteur(1,0), Vecteur(), 1.0));
    u.ajouterParticule(Particule(Vecteur(2,0), Vecteur(), 1.0));
    EXPECT_EQ(u.getNbParticules(), 2);
}

TEST(UniversTest, AccesHorsLimites) {
    Univers u;
    EXPECT_THROW(u.getParticule(0), std::out_of_range);
}

TEST(UniversTest, ForceGravitationnelle) {
    Univers u(2, {10.0, 10.0}, 0.0, TypeForce::GRAVITATIONNELLE);
    u.ajouterParticule(Particule(Vecteur(0,0), Vecteur(), 1.0));
    u.ajouterParticule(Particule(Vecteur(1,0), Vecteur(), 1.0));
    u.calculerForces();
    EXPECT_NEAR(u.getParticule(0).getForce().getX(), 1.0, 1e-10);
    EXPECT_NEAR(u.getParticule(1).getForce().getX(), -1.0, 1e-10);
}

TEST(UniversTest, EnergieCinetique) {
    Univers u;
    u.ajouterParticule(Particule(Vecteur(), Vecteur(3,4,0), 2.0));
    EXPECT_DOUBLE_EQ(u.energieCinetique(), 25.0);
}

TEST(UniversTest, InitialisationGrille) {
    Univers u(2, {10.0, 10.0}, 2.5, TypeForce::LENNARD_JONES);
    u.initialiserGrille();
    u.ajouterParticule(Particule(Vecteur(1,1), Vecteur(), 1.0));
    u.assignerParticulesCellules();
    EXPECT_GE(u.getParticule(0).getCellule(), 0);
}

TEST(UniversTest, LennardJonesRepulsif) {
    Univers u(2, {10.0, 10.0}, 2.5, TypeForce::LENNARD_JONES);
    u.setLennardJonesParams(1.0, 1.0);
    u.ajouterParticule(Particule(Vecteur(0,0), Vecteur(), 1.0));
    u.ajouterParticule(Particule(Vecteur(0.5,0), Vecteur(), 1.0));
    u.calculerForces();
    EXPECT_LT(u.getParticule(0).getForce().getX(), 0.0);
}

TEST(UniversTest, SortieVTK) {
    Univers u;
    u.ajouterParticule(Particule(Vecteur(1,2,3), Vecteur(0.1,0.2,0.3), 1.0));
    EXPECT_NO_THROW(u.ecrireVTK("/tmp/test_output.vtu"));
}
