cmake_minimum_required(VERSION 3.14)
project(TowerDefense LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- CONFIGURATION DES DEPENDANCES ---
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# --- DOSSIERS D'INCLUSION (HEADERS) ---
# Permet à VS Code et au compilateur de trouver "include/nlohmann/json.hpp"
# en écrivant simplement #include <nlohmann/json.hpp>
include_directories(include)
include_directories(src)
include_directories(src/mob)

# --- DECLARATION DE L'EXECUTABLE ---
# On liste explicitement tous les fichiers visibles dans votre dossier 'src' et 'src/mob'
add_executable(TowerDefense 
    src/main.cpp 
    src/Map.cpp
    src/mob/Monster.cpp
    src/mob/Skeleton.cpp
    src/Tour.cpp
    src/Projectile.cpp
)

# Copier automatiquement UNIQUEMENT le dossier Map dans le dossier build lors de la compilation
add_custom_command(TARGET TowerDefense POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/Map"
    "${CMAKE_BINARY_DIR}/Map"
)

# --- LIAISON DES BIBLIOTHÈQUES (LINKING) ---
target_link_libraries(TowerDefense PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
)