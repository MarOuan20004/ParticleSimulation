#!/usr/bin/env bash
# ==============================================================================
# Script de compilation pour ParticleSimulation
# Usage: ./scripts/build.sh [Release|Debug]
# ==============================================================================

set -euo pipefail

BUILD_TYPE="${1:-Release}"
BUILD_DIR="build"
NPROC=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "=== Configuration du projet (${BUILD_TYPE}) ==="
cmake -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DBUILD_TESTING=ON \
    -DBUILD_DEMOS=ON \
    -DBUILD_DOCS=ON

echo "=== Compilation sur ${NPROC} coeurs ==="
cmake --build "${BUILD_DIR}" --config "${BUILD_TYPE}" -j"${NPROC}"

echo "=== Exécution de la suite de tests (CTest) ==="
ctest --test-dir "${BUILD_DIR}" --output-on-failure

echo "Compilation et tests réussis avec succès."
