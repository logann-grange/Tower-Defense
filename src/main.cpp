#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "Map/Map.hpp"
#include "mob/logic/SkeletonMage.hpp"
#include "mob/view/SkeletonMageView.hpp"

// Structure pour lier la logique et le visuel de chaque monstre
struct MonstreInstance {
    std::unique_ptr<Monster> logique;
    std::unique_ptr<MonsterView> graphique;
    bool recompenseDonnee = false; // Évite de donner l'or en boucle pendant qu'il tombe
};

int main() {
    // 1. FENÊTRE ET DIMENSIONS
    sf::RenderWindow window(sf::VideoMode({1376, 1056}), "Tower Defence - Un Seul Monstre");

    Map mon_niveau;
    if (!mon_niveau.loadFromFile("Map/map_tower_defence.ldtk")) {
        std::cerr << "Erreur : Impossible de charger le fichier LDTK !" << std::endl;
        return -1;
    }

    // La caméra filme EXACTEMENT la taille de ta carte (688x528)
    sf::View vueJeu(sf::FloatRect({0.f, 0.f}, {688.f, 528.f}));
    window.setView(vueJeu);

    // Récupération du chemin pour les monstres
    sf::Vector2i caseDepart = mon_niveau.trouverPointDepartDepuisEntite(); 
    std::vector<sf::Vector2i> cheminMonstres = mon_niveau.genererChemin(caseDepart);

    // 2. ÉCONOMIE ET STATS DU JOUEUR
    int orJoueur = 100;
    int pvJoueur = 20;

    // Tableau dynamique (vector) pour stocker nos monstres
    std::vector<MonstreInstance> listeMonstres;
    sf::Clock clock;

    // =======================================================
    // CHANGEMENT ICI : On crée UN SEUL monstre au lieu de 5
    // =======================================================
    MonstreInstance m;
    m.logique = std::make_unique<SkeletonMage>();
    m.graphique = std::make_unique<SkeletonMageView>();
    
    m.logique->spawn(cheminMonstres);
    listeMonstres.push_back(std::move(m)); // Ajout de l'unique squelette

    // 3. BOUCLE PRINCIPALE DU JEU
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            // ZONE DE TEST : Si tu appuies sur ESPACE, le tout premier monstre prend des dégâts
            if (event->is<sf::Event::KeyPressed>() && 
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                if (!listeMonstres.empty() && !listeMonstres[0].logique->estMort()) {
                    // On lui met 20 dégâts d'Explosion (ce qui fera 40 car c'est sa faiblesse !)
                    listeMonstres[0].logique->subirDegats(20.f, "Explosion"); 
                }
            }
        }

        // --- A. MISE À JOUR LOGIQUE ET VISUELLE ---
        for (auto& monstre : listeMonstres) {
            // On fait avancer le monstre UNIQUEMENT s'il est vivant
            if (!monstre.logique->estMort()) {
                monstre.logique->move(deltaTime); 

                // Si le monstre passe la ligne d'arrivée
                if (monstre.logique->doitInfligerDegats()) {
                    pvJoueur -= monstre.logique->getDegatsBase();
                    std::cout << "Aie ! Base touchee ! PV restants : " << pvJoueur << std::endl;
                }
            }
            else {
                // Si le monstre est mort mais qu'on ne lui a pas encore pris ses PO
                if (!monstre.recompenseDonnee) {
                    orJoueur += monstre.logique->getOrRecompense();
                    monstre.recompenseDonnee = true; // Verrouille pour ne le faire qu'une seule fois
                    std::cout << "Monstre tue ! + " << monstre.logique->getOrRecompense() 
                              << " Or. (Total: " << orJoueur << ")" << std::endl;
                }
            }
            
            // Le visuel s'actualise TOUJOURS (permet de faire défiler le Death-Sheet)
            monstre.graphique->update(deltaTime, *(monstre.logique));
        }

        // --- B. NETTOYAGE INTELLIGENT DE LA MÉMOIRE (Le Culling) ---
        for (auto it = listeMonstres.begin(); it != listeMonstres.end(); ) {
            
            // On récupère le pointeur de la vue et on le convertit en SkeletonMageView pour lire sa variable de mort
            auto squeletteGraphique = dynamic_cast<SkeletonMageView*>(it->graphique.get());

            // CAS 1 : Le monstre est mort ET son animation de mort au sol est 100% terminée
            if (it->logique->estMort() && squeletteGraphique && squeletteGraphique->estPretADetruire()) {
                it = listeMonstres.erase(it); // Supprime proprement le monstre du vector et de la RAM
            } 
            // CAS 2 : Le monstre a passé la ligne d'arrivée vivant, il disparaît de l'écran
            else if (it->logique->estArrive()) {
                it = listeMonstres.erase(it); 
            } 
            // CAS 3 : Il avance encore ou est en train de jouer son effondrement, on le laisse tranquille
            else {
                ++it;
            }
        }

        // --- C. RENDU GRAPHIQUE ---
        window.clear(sf::Color(40, 44, 52)); 
        window.setView(vueJeu);

        // Dessin du niveau LDtk
        mon_niveau.draw(window);

        // Dessin des monstres (il n'y en aura qu'un seul maintenant !)
        for (auto& monstre : listeMonstres) {
            monstre.graphique->draw(window);
        }

        window.display();
    }

    return 0;
}