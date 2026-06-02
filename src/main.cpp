#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"
#include "mob/Skeleton.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Tower Defence - Vagues de Squelettes");

    Map mon_niveau;
    if (!mon_niveau.loadFromFile("Map/map_tower_defence.ldtk")) {
        return -1;
    }

    // -------------------------------------------------------------------------
    // CORRECTION : AJOUT DE LA VUE (CAMÉRA) POUR SUPPRIMER LES BORDURES NOIRES
    // -------------------------------------------------------------------------
    // On récupère la taille de la carte en nombre de cases, multipliée par la taille d'une case (16px)
    float carte_largeur_pixels = static_cast<float>(mon_niveau.getWidth() * 16);
    float carte_hauteur_pixels = static_cast<float>(mon_niveau.getHeight() * 16);

    // On crée une caméra qui fait la taille logique exacte de la carte
    sf::View camera(sf::FloatRect({0.f, 0.f}, {carte_largeur_pixels, carte_hauteur_pixels}));
    
    // On dit à la fenêtre d'utiliser cette caméra (SFML va l'étirer pour remplir le 800x600)
    window.setView(camera);
    // -------------------------------------------------------------------------

    // 1. GENERER LE CHEMIN
    // Remplacez {0, 5} par les coordonnées de la case exactes où commence votre chemin dans LDtk !
    sf::Vector2i caseDepart(0, 5); 
    std::vector<sf::Vector2i> cheminMonstres = mon_niveau.genererChemin(caseDepart);

    // 2. CREER LE SQUELETTE
    Skeleton monSquelette;
    monSquelette.spawn(cheminMonstres); // Il se téléporte automatiquement au départ !

    // Horloge pour mesurer le temps réel écoulé (indispensable pour move(deltaTime))
    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        // --- MISE À JOUR LOGIQUE ---
        monSquelette.move(deltaTime); // Le squelette avance tout seul sur les rails du chemin !

        // --- AFFICHAGE ---
        window.clear();
        
        mon_niveau.draw(window); // On dessine la map (Sol, décors...)
        monSquelette.draw(window); // On dessine le squelette par-dessus

        window.display();
    }

    return 0;
}