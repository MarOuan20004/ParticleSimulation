// test_particule.cxx - tests pour Particule

#include <gtest/gtest.h>
#include "Particule.hxx"
#include <cmath>

TEST(ParticuleTest, ConstructeurDefaut) {
    Particule p;
    EXPECT_DOUBLE_EQ(p.getMasse(), 1.0);
    EXPECT_EQ(p.getPosition(), Vecteur(0.0, 0.0, 0.0));
    EXPECT_EQ(p.getVitesse(), Vecteur(0.0, 0.0, 0.0));
    EXPECT_EQ(p.getForce(), Vecteur(0.0, 0.0, 0.0));
}

TEST(ParticuleTest, ConstructeurParametre) {
    Vecteur pos(1.0, 2.0, 3.0);
    Vecteur vit(0.1, 0.2, 0.3);
    Particule p(pos, vit, 5.0, 2);
    EXPECT_EQ(p.getPosition(), pos);
    EXPECT_EQ(p.getVitesse(), vit);
    EXPECT_DOUBLE_EQ(p.getMasse(), 5.0);
    EXPECT_EQ(p.getCategorie(), 2);
}

TEST(ParticuleTest, IdentifiantsUniques) {
    Particule p1;
    Particule p2;
    EXPECT_NE(p1.getId(), p2.getId());
}

TEST(ParticuleTest, GestionForce) {
    Particule p;
    p.ajouterForce(Vecteur(1.0, 0.0, 0.0));
    p.ajouterForce(Vecteur(0.0, 2.0, 0.0));
    EXPECT_EQ(p.getForce(), Vecteur(1.0, 2.0, 0.0));

    p.resetForce();
    EXPECT_EQ(p.getForce(), Vecteur(0.0, 0.0, 0.0));
}

TEST(ParticuleTest, SauvegardeForce) {
    Particule p;
    p.ajouterForce(Vecteur(1.0, 2.0, 3.0));
    p.sauvegarderForce();
    EXPECT_EQ(p.getForceOld(), Vecteur(1.0, 2.0, 3.0));
}

TEST(ParticuleTest, EnergieCinetique) {
    Vecteur vit(3.0, 4.0, 0.0); // norme = 5
    Particule p(Vecteur(), vit, 2.0);
    // Ec = 0.5 * m * v² = 0.5 * 2 * 25 = 25
    EXPECT_DOUBLE_EQ(p.energieCinetique(), 25.0);
}

TEST(ParticuleTest, UpdatePosition) {
    Vecteur pos(0.0, 0.0, 0.0);
    Vecteur vit(1.0, 0.0, 0.0);
    Particule p(pos, vit, 1.0);
    p.ajouterForce(Vecteur(2.0, 0.0, 0.0)); // F = 2

    double dt = 0.1;
    // x = x + dt * (v + 0.5/m * F * dt)
    // x = 0 + 0.1 * (1 + 0.5 * 2 * 0.1) = 0.1 * 1.1 = 0.11
    p.updatePosition(dt);
    EXPECT_NEAR(p.getPosition().getX(), 0.11, 1e-10);
}

TEST(ParticuleTest, UpdateVitesse) {
    Particule p(Vecteur(), Vecteur(1.0, 0.0, 0.0), 1.0);
    p.ajouterForce(Vecteur(2.0, 0.0, 0.0));  // F_new = 2
    p.setForceOld(Vecteur(4.0, 0.0, 0.0));   // F_old = 4

    double dt = 0.1;
    // v = v + dt * 0.5/m * (F + F_old)
    // v = 1 + 0.1 * 0.5 * (2 + 4) = 1 + 0.3 = 1.3
    p.updateVitesse(dt);
    EXPECT_NEAR(p.getVitesse().getX(), 1.3, 1e-10);
}

TEST(ParticuleTest, Setters) {
    Particule p;
    p.setPosition(Vecteur(1.0, 2.0, 3.0));
    p.setVitesse(Vecteur(4.0, 5.0, 6.0));
    EXPECT_EQ(p.getPosition(), Vecteur(1.0, 2.0, 3.0));
    EXPECT_EQ(p.getVitesse(), Vecteur(4.0, 5.0, 6.0));
}

TEST(ParticuleTest, ConstructeurCopie) {
    Particule p1(Vecteur(1.0, 2.0), Vecteur(3.0, 4.0), 5.0, 7);
    Particule p2(p1);
    EXPECT_EQ(p2.getPosition(), p1.getPosition());
    EXPECT_EQ(p2.getVitesse(), p1.getVitesse());
    EXPECT_DOUBLE_EQ(p2.getMasse(), p1.getMasse());
    EXPECT_EQ(p2.getCategorie(), p1.getCategorie());
}
