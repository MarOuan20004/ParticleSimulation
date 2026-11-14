/// Code élémentaire pour le calcul de la trace d'une matrice.
/// Printemps 2026
/// Objectif : manipuler les éléments de base du C++
///            utiliser des outils de profiling et de débogage
/// ref: l'exercice est inspiré très largement de ressources WWW

#include <cmath>
#include <cstdlib>
#include <iostream>

double **initialization(int);
double *fill_vector(double *, int);  // Corrigé : fill_vectors -> fill_vector
void print_matrix(double **, int);
double trace(double **, int);       // Corrigé : retourne double au lieu de double*
using std::cout;

/// Driver principal pour le calcul de la tace d'une matrice
int main() {
  int i, j, n;
  double **matrix;
  double sum;  // Corrigé : double au lieu de double*

  std::cout << "Enter the Dimension for a square matrix: " << std::endl << std::flush;
  std::cin >> n;
  matrix = initialization(n);
  for (i = 0; i < n; i++)  // Corrigé : i=0 au lieu de i=1
    matrix[i] = fill_vector(matrix[i], n);
  sum = trace(matrix, n);
  print_matrix(matrix, n);
  std::cout << "Sum of the diagonal elements are: " << sum << std::endl;

  // Corrigé : libération mémoire
  for (i = 0; i < n; i++)
    delete[] matrix[i];
  delete[] matrix;

  return 0;
}

/// @brief Routine d'initialization qui permet d'allouer la mémoire pour une
///        matrice carrée de taille n et renvoie un pointer vers
///        la matrice allouée
/// @param[in] n est la taille souhaitée de la matrice
/// @return    Renvoie le pointeur vers la matrice allouée
double **initialization(int n) {
  int i;
  double **matrix;
  matrix = new double*[n];               // Corrigé : calloc -> new (C++)
  for (i = 0; i < n; ++i)
    matrix[i] = new double[n]();         // Corrigé : calloc -> new (C++)
  return (matrix);
}

/// @brief Intitialise un vecteur avec des valeurs aléatoires comprises
///        entre dans l'intervalle [-10;10]
/// @param[in] vec est le vecteur à initialiser
/// @param[in] n   est la taille du vecteur à initialiser
/// @return    Renvoie le vecteur initialisé.
double *fill_vector(double *vec, int n) {
  int i;
  for (i = 0; i < n; i++)
    vec[i] = rand() % 20 - 10;
  return vec;  // Corrigé : &vec -> vec
}

/// @brief Affiche les éléments d'une matrice de taille n
/// @param[in] matrix est la matrice à afficher
/// @param[in] n est la taille de la matrice à afficher
void print_matrix(double **matrix, int n) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++)
      cout << matrix[i][j] << ", ";
    cout << std::endl;  // Corrigé : endl -> std::endl
  }
}

/// @brief Calcul la trace d'une matrice
/// @param[in] matrix est la matrice dont on souhaite connaître la trace.
/// @param[in] n est la taille de la matrice.
double trace(double **matrix, int n) {  // Corrigé : retourne double
  int i;
  double sum = 0.0;
  for (i = 0; i < n; i++)     // Corrigé : i=0..n-1 au lieu de i=1..n
    sum += matrix[i][i];
  return sum;                  // Corrigé : retourne la valeur, pas &sum
}
