## Génération de la documentation Doxygen

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxyfilerc)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Configurer le fichier Doxygen
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Cible 'doc' pour générer la documentation
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Génération de la documentation Doxygen..."
        VERBATIM
    )
else()
    message(WARNING "Doxygen non trouvé. La cible 'doc' ne sera pas disponible.")
endif()
