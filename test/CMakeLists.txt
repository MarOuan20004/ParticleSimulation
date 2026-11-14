# Pour chaque test (famille de test), un exécutable est créé
add_executable(test_vecteur test_vecteur.cxx)
target_link_libraries(test_vecteur gtest_main particules)

add_executable(test_particule test_particule.cxx)
target_link_libraries(test_particule gtest_main particules)

add_executable(test_univers test_univers.cxx)
target_link_libraries(test_univers gtest_main particules)

add_executable(test_boundary test_boundary.cxx)
target_link_libraries(test_boundary gtest_main particules)

include(GoogleTest)
gtest_discover_tests(test_vecteur)
gtest_discover_tests(test_particule)
gtest_discover_tests(test_univers)
gtest_discover_tests(test_boundary)
