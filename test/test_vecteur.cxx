// test_vecteur.cxx - tests Google Test pour Vecteur

#include <gtest/gtest.h>
#include "Vecteur.hxx"
#include <cmath>

TEST(VecteurTest, ConstructeurDefaut) {
    Vecteur v;
    EXPECT_DOUBLE_EQ(v.getX(), 0.0);
    EXPECT_DOUBLE_EQ(v.getY(), 0.0);
    EXPECT_DOUBLE_EQ(v.getZ(), 0.0);
}

TEST(VecteurTest, Constructeur3D) {
    Vecteur v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.getX(), 1.0);
    EXPECT_DOUBLE_EQ(v.getY(), 2.0);
    EXPECT_DOUBLE_EQ(v.getZ(), 3.0);
}

TEST(VecteurTest, Constructeur2D) {
    Vecteur v(4.0, 5.0);
    EXPECT_DOUBLE_EQ(v.getX(), 4.0);
    EXPECT_DOUBLE_EQ(v.getY(), 5.0);
    EXPECT_DOUBLE_EQ(v.getZ(), 0.0);
}

TEST(VecteurTest, ConstructeurCopie) {
    Vecteur v1(1.0, 2.0, 3.0);
    Vecteur v2(v1);
    EXPECT_EQ(v2, v1);
}

TEST(VecteurTest, Affectation) {
    Vecteur v1(1.0, 2.0, 3.0);
    Vecteur v2;
    v2 = v1;
    EXPECT_EQ(v2, v1);
}

TEST(VecteurTest, Addition) {
    Vecteur v1(1.0, 2.0, 3.0);
    Vecteur v2(4.0, 5.0, 6.0);
    Vecteur v3 = v1 + v2;
    EXPECT_DOUBLE_EQ(v3.getX(), 5.0);
    EXPECT_DOUBLE_EQ(v3.getY(), 7.0);
    EXPECT_DOUBLE_EQ(v3.getZ(), 9.0);
}

TEST(VecteurTest, Soustraction) {
    Vecteur v1(4.0, 5.0, 6.0);
    Vecteur v2(1.0, 2.0, 3.0);
    Vecteur v3 = v1 - v2;
    EXPECT_DOUBLE_EQ(v3.getX(), 3.0);
    EXPECT_DOUBLE_EQ(v3.getY(), 3.0);
    EXPECT_DOUBLE_EQ(v3.getZ(), 3.0);
}

TEST(VecteurTest, MultiplicationScalaire) {
    Vecteur v(1.0, 2.0, 3.0);
    Vecteur v2 = v * 2.0;
    EXPECT_DOUBLE_EQ(v2.getX(), 2.0);
    EXPECT_DOUBLE_EQ(v2.getY(), 4.0);
    EXPECT_DOUBLE_EQ(v2.getZ(), 6.0);

    Vecteur v3 = 3.0 * v;
    EXPECT_DOUBLE_EQ(v3.getX(), 3.0);
    EXPECT_DOUBLE_EQ(v3.getY(), 6.0);
    EXPECT_DOUBLE_EQ(v3.getZ(), 9.0);
}

TEST(VecteurTest, DivisionScalaire) {
    Vecteur v(6.0, 4.0, 2.0);
    Vecteur v2 = v / 2.0;
    EXPECT_DOUBLE_EQ(v2.getX(), 3.0);
    EXPECT_DOUBLE_EQ(v2.getY(), 2.0);
    EXPECT_DOUBLE_EQ(v2.getZ(), 1.0);
}

TEST(VecteurTest, DivisionParZero) {
    Vecteur v(1.0, 2.0, 3.0);
    EXPECT_THROW(v / 0.0, std::runtime_error);
}

TEST(VecteurTest, Negation) {
    Vecteur v(1.0, -2.0, 3.0);
    Vecteur v2 = -v;
    EXPECT_DOUBLE_EQ(v2.getX(), -1.0);
    EXPECT_DOUBLE_EQ(v2.getY(), 2.0);
    EXPECT_DOUBLE_EQ(v2.getZ(), -3.0);
}

TEST(VecteurTest, OperateursComposes) {
    Vecteur v(1.0, 2.0, 3.0);
    v += Vecteur(1.0, 1.0, 1.0);
    EXPECT_EQ(v, Vecteur(2.0, 3.0, 4.0));

    v -= Vecteur(1.0, 1.0, 1.0);
    EXPECT_EQ(v, Vecteur(1.0, 2.0, 3.0));

    v *= 2.0;
    EXPECT_EQ(v, Vecteur(2.0, 4.0, 6.0));

    v /= 2.0;
    EXPECT_EQ(v, Vecteur(1.0, 2.0, 3.0));
}

TEST(VecteurTest, Norme) {
    Vecteur v(3.0, 4.0, 0.0);
    EXPECT_DOUBLE_EQ(v.norme(), 5.0);
    EXPECT_DOUBLE_EQ(v.normeCarree(), 25.0);
}

TEST(VecteurTest, ProduitScalaire) {
    Vecteur v1(1.0, 2.0, 3.0);
    Vecteur v2(4.0, 5.0, 6.0);
    EXPECT_DOUBLE_EQ(v1.dot(v2), 32.0); // 4+10+18
}

TEST(VecteurTest, ProduitVectoriel) {
    Vecteur ex(1.0, 0.0, 0.0);
    Vecteur ey(0.0, 1.0, 0.0);
    Vecteur ez = ex.cross(ey);
    EXPECT_DOUBLE_EQ(ez.getX(), 0.0);
    EXPECT_DOUBLE_EQ(ez.getY(), 0.0);
    EXPECT_DOUBLE_EQ(ez.getZ(), 1.0);
}

TEST(VecteurTest, Comparaison) {
    Vecteur v1(1.0, 2.0, 3.0);
    Vecteur v2(1.0, 2.0, 3.0);
    Vecteur v3(1.0, 2.0, 3.1);
    EXPECT_EQ(v1, v2);
    EXPECT_NE(v1, v3);
}

TEST(VecteurTest, Setters) {
    Vecteur v;
    v.setX(1.0);
    v.setY(2.0);
    v.setZ(3.0);
    EXPECT_EQ(v, Vecteur(1.0, 2.0, 3.0));
}
