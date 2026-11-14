#!/usr/bin/env bash
# ==============================================================================
# Script d'exécution de l'ensemble des démonstrations
# Usage: ./scripts/run_all_demos.sh
# ==============================================================================

set -euo pipefail

BUILD_DIR="build"
OUTPUT_DIR="${BUILD_DIR}/outputs"

if [ ! -d "${BUILD_DIR}/demo" ]; then
    echo "Erreur: Le répertoire ${BUILD_DIR}/demo n'existe pas. Lancez d'abord ./scripts/build.sh"
    exit 1
fi

mkdir -p "${OUTPUT_DIR}"
cd "${OUTPUT_DIR}"

echo "=========================================================="
echo "  1. Exécution TP1 - Trace matricielle"
echo "=========================================================="
echo "3" | "../demo/demo_tp1_trace"

echo ""
echo "=========================================================="
echo "  2. Exécution TP1 - EDO Euler Explicite / Implicite"
echo "=========================================================="
"../demo/demo_tp1_euler"

echo ""
echo "=========================================================="
echo "  3. Exécution TP2 - Système Solaire (Störmer-Verlet)"
echo "=========================================================="
"../demo/demo_tp2_solar"

echo ""
echo "=========================================================="
echo "  4. Exécution TP3 - Benchmark Collections STL"
echo "=========================================================="
"../demo/demo_tp3_collections"

echo ""
echo "=========================================================="
echo "  5. Exécution TP4 - Collision Moléculaire (Lennard-Jones)"
echo "=========================================================="
"../demo/demo_tp4_collision"

echo ""
echo "=========================================================="
echo "  6. Exécution TP6 - Conditions Limites & Thermostat"
echo "=========================================================="
"../demo/demo_tp6_boundary"

echo ""
echo "Toutes les démonstrations ont été exécutées avec succès."
echo "Les fichiers VTK (.vtu) sont disponibles dans : ${OUTPUT_DIR}"
