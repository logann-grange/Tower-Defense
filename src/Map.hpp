#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>

struct TileInfo {
    int x, y;
    int srcX,srcY;
    sf::Texture* texture;
};

class Map
{
    public:
        Map();
        ~Map()=default;
        bool loadFromFile(const std::string& ldtkFilename);
        bool isMonsterPath(int x, int y) const;
        bool isTowerZone(int x, int y) const;

        int getWidth() const;
        int getHeight() const;

        std::vector<sf::Vector2i> genererChemin(sf::Vector2i pointDepart);

        void draw(sf::RenderWindow& window);

    private:
        int m_width;
        int m_height;
        int m_gridSize;
        std::vector<std::vector<int>> m_grid;

        std::map<std::string, sf::Texture> m_textures;
        std::vector<std::vector<TileInfo>> m_layerTiles; // Stocke les tuiles de chaque calque

        inline static const int VAL_VIDE = 0;
        inline static const int VAL_CHEMIN = 2; 
        inline static const int VAL_TOURS = 3;
};

#endif // MAP_HPP