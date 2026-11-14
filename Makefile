# ==============================================================================
# Makefile d'automatisation pour le projet ParticleSimulation
# ==============================================================================

.PHONY: all build release debug test doc valgrind format run-demos clean help

BUILD_DIR ?= build
BUILD_TYPE ?= Release

all: build

help:
	@echo "Cibles disponibles :"
	@echo "  make build      - Compile le projet en mode Release"
	@echo "  make debug      - Compile le projet en mode Debug"
	@echo "  make test       - Exécute l'ensemble des tests unitaires (CTest)"
	@echo "  make doc        - Génère la documentation Doxygen dans doc/html/"
	@echo "  make valgrind   - Vérifie l'absence de fuites mémoire avec Valgrind"
	@echo "  make format     - Formate le code source avec clang-format"
	@echo "  make run-demos  - Exécute les démonstrations numériques"
	@echo "  make clean      - Supprime le répertoire de build et les fichiers générés"

build:
	@./scripts/build.sh Release

debug:
	@./scripts/build.sh Debug

test:
	@ctest --test-dir $(BUILD_DIR) --output-on-failure

doc:
	@cmake --build $(BUILD_DIR) --target doc
	@echo "Documentation générée. Ouvrir $(BUILD_DIR)/doc/html/index.html dans un navigateur."

valgrind:
	@./scripts/check_memory.sh

format:
	@find include src demo test -name "*.hxx" -o -name "*.cxx" | xargs clang-format -i
	@echo "Formatage terminé."

run-demos:
	@./scripts/run_all_demos.sh

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f *.vtu
	@echo "Nettoyage terminé."
