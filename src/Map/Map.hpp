#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>



class Map {
public:
    Map();
    bool loadFromFile(const std::string& ldtkFilename);
    void draw(sf::RenderWindow& window);
    
    bool isMonsterPath(int x, int y) const;
    bool isTowerZone(int x, int y) const;
    
    std::vector<sf::Vector2i> genererChemin(sf::Vector2i pointDepart);
    sf::Vector2i trouverPointDepartDepuisEntite() const;
    sf::Vector2i getPositionObjectif() const;

    int getGridValue(int x, int y) const;

    int getWidth() const ;
    int getHeight() const;

private:
    // Structure interne pour stocker les informations de chaque tuile graphique
    struct TileInfo {
        int x, y;
        int srcX, srcY;
        sf::Texture* texture;
    };

    int m_width;
    int m_height;
    int m_gridSize;
    
    sf::Vector2i m_pointDepart; // Contiendra le spawn automatique

    // Constantes de l'IntGrid synchronisées avec ton fichier .ldtk
    inline static const int VAL_VIDE = 0;
    inline static const int VAL_TOURS = 3;     // Zone constructible pour les tours
    inline static const int VAL_CHEMIN = 2;    // "zone_de_chemin" dans ton LDtk

    sf::Vector2i m_positionObjectif; // Contiendra la position de la ligne d'arrivée (objectif final des monstres)

    std::vector<std::vector<int>> m_grid; // Grille de logique (IntGrid)
    std::vector<std::vector<TileInfo>> m_layerTiles; // Couches visuelles
    std::map<std::string, sf::Texture> m_textures; // Cache de textures
};

#endif

