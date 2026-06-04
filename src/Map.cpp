#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"
#include "Map.hpp"

using json = nlohmann::json;

Map::Map() : m_width(0), m_height(0), m_gridSize(16), m_pointDepart(0, 5) {
}

bool Map::loadFromFile(const std::string& ldtkFilename) {
    std::ifstream file(ldtkFilename);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir " << ldtkFilename << std::endl;
        return false;
    }

    json ldtk_data;
    file >> ldtk_data;

    // Récupération du premier niveau
    auto& level = ldtk_data["levels"][0];
    
    m_layerTiles.clear();

    // On parcourt tous les calques pour tout charger d'un coup
    for (auto& layer : level["layerInstances"]) {
        
        // A. Calque de LOGIQUE (IntGrid)
        if (layer["__identifier"] == "IntGrid") {
            m_width = layer["__cWid"].get<int>();
            m_height = layer["__cHei"].get<int>();
            auto flat_grid = layer["intGridCsv"].get<std::vector<int>>();
            
            m_grid.assign(m_height, std::vector<int>(m_width, VAL_VIDE));
            for (int y = 0; y < m_height; ++y) {
                for (int x = 0; x < m_width; ++x) {
                    m_grid[y][x] = flat_grid[y * m_width + x];
                }
            }
        }
        
        // B. Calque d'ENTITÉS (Recherche du Spawn)
        if (layer["__type"] == "Entities") {
            for (auto& entity : layer["entityInstances"]) {
                // Correspond à l'identifiant "Monster_spawn" présent dans ton fichier .ldtk
                if (entity["__identifier"] == "Monster_spawn") {
                    m_pointDepart.x = entity["__grid"][0].get<int>();
                    m_pointDepart.y = entity["__grid"][1].get<int>();
                    std::cout << "SUCCESS : Entite Monster_spawn trouvee en case : (" 
                              << m_pointDepart.x << ", " << m_pointDepart.y << ")\n";
                }
            }
        }
        
        // C. Calques VISUELS (Tiles ou AutoLayer)
        if ((layer["__type"] == "Tiles" || layer["__type"] == "AutoLayer") && !layer["gridTiles"].empty()) {
            m_gridSize = layer["__gridSize"].get<int>();

            std::string texturePath = layer["__tilesetRelPath"].get<std::string>();
            if (texturePath.rfind("../", 0) == 0) {
                texturePath = texturePath.substr(3);
            }

            if (m_textures.find(texturePath) == m_textures.end()) {
                sf::Texture tex;
                if (!tex.loadFromFile(texturePath)) {
                    std::cerr << "Erreur : Impossible de charger le PNG : " << texturePath << std::endl;
                    return false;
                }
                m_textures[texturePath] = tex;
            }

            sf::Texture* texturePointeur = &m_textures[texturePath];
            std::vector<TileInfo> currentLayerTiles;

            for (auto& tile : layer["gridTiles"]) {
                TileInfo info;
                info.x = tile["px"][0].get<int>() / m_gridSize;
                info.y = tile["px"][1].get<int>() / m_gridSize;
                info.srcX = tile["src"][0].get<int>();
                info.srcY = tile["src"][1].get<int>();
                info.texture = texturePointeur;
                currentLayerTiles.push_back(info);
            }

            m_layerTiles.push_back(currentLayerTiles);
        }
    }
    return true;
}

sf::Vector2i Map::trouverPointDepartDepuisEntite() const {
    return m_pointDepart;
}

void Map::draw(sf::RenderWindow& window) {
    for (int i = static_cast<int>(m_layerTiles.size()) - 1; i >= 0; --i) {
        for (const auto& tile : m_layerTiles[i]) {
            if (tile.texture != nullptr) {
                sf::Sprite sprite(*tile.texture); 
                
                sf::Vector2i texturePosition(tile.srcX, tile.srcY);
                sf::Vector2i textureTaille(m_gridSize, m_gridSize);
                sprite.setTextureRect(sf::IntRect(texturePosition, textureTaille));
                
                sf::Vector2f positionEcran(
                    static_cast<float>(tile.x * m_gridSize), 
                    static_cast<float>(tile.y * m_gridSize)
                );
                sprite.setPosition(positionEcran);
                
                window.draw(sprite);
            }
        }
    }
}

bool Map::isMonsterPath(int x, int y) const {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        return m_grid[y][x] == VAL_CHEMIN; // VAL_CHEMIN vaut 2 (zone_de_chemin)
    }
    return false;
}

bool Map::isTowerZone(int x, int y) const {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        return m_grid[y][x] == VAL_TOURS;
    }
    return false;
}

std::vector<sf::Vector2i> Map::genererChemin(sf::Vector2i pointDepart) {
    std::vector<sf::Vector2i> chemin;
    sf::Vector2i caseActuelle = pointDepart;
    chemin.push_back(caseActuelle);

    std::vector<sf::Vector2i> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    
    bool cheminEnCours = true;
    int indexSecurite = 0; 

    while (cheminEnCours && indexSecurite < 1000) {
        indexSecurite++;
        bool caseTrouvee = false;

        for (const auto& dir : directions) {
            sf::Vector2i voisine = caseActuelle + dir;

            if (isMonsterPath(voisine.x, voisine.y)) {
                // Permet d'avancer vers la suite même si la case actuelle est le spawn d'entité
                if (chemin.size() < 2 || voisine != chemin[chemin.size() - 2]) {
                    caseActuelle = voisine;
                    chemin.push_back(caseActuelle);
                    caseTrouvee = true;
                    break;
                }
            }
        }

        if (!caseTrouvee) {
            cheminEnCours = false;
        }
    }

    return chemin;
}
int Map::getWidth() const { return m_width; }
int Map::getHeight() const { return m_height; }