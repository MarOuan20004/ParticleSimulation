# Bibliothèque principale de simulation de particules
add_library(particules
    Vecteur.cxx
    Particule.cxx
    Cellule.cxx
    Univers.cxx
)

target_include_directories(particules PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
