#!/usr/bin/env bash
# ==============================================================================
# Script de vérification mémoire avec Valgrind (zéro fuite tolérée)
# Usage: ./scripts/check_memory.sh
# ==============================================================================

set -euo pipefail

BUILD_DIR="build"

if [ ! -d "${BUILD_DIR}/test" ]; then
    echo "Erreur: Les exécutables de test n'existent pas dans ${BUILD_DIR}/test."
    echo "Compilez d'abord avec: ./scripts/build.sh Debug"
    exit 1
fi

TESTS=(
    "${BUILD_DIR}/test/test_vecteur"
    "${BUILD_DIR}/test/test_particule"
    "${BUILD_DIR}/test/test_boundary"
    "${BUILD_DIR}/test/test_univers"
)

VALGRIND_FLAGS=(
    --leak-check=full
    --show-leak-kinds=all
    --track-origins=yes
    --error-exitcode=1
)

echo "=== Démarrage de l'analyse mémoire Valgrind ==="

for test_bin in "${TESTS[@]}"; do
    echo ""
    echo "----------------------------------------------------------"
    echo "Vérification : ${test_bin}"
    echo "----------------------------------------------------------"
    valgrind "${VALGRIND_FLAGS[@]}" "${test_bin}"
done

echo ""
echo "Toutes les vérifications Valgrind ont réussi sans aucune erreur mémoire."
