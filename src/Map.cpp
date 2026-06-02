#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"
#include "Map.hpp"

using json =nlohmann::json;

// (Note : Les lignes "const int Map::VAL_..." ont été supprimées d'ici)

Map::Map() : m_width(0), m_height(0) {
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
    
    std::vector<int> flat_grid;
    m_layerTiles.clear();

    // 3. On parcourt tous les calques pour tout charger d'un coup
    for (auto& layer : level["layerInstances"]) {
        
        // A. S'il s'agit du calque de LOGIQUE (IntGrid)
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
        
        // B. S'il s'agit d'un calque VISUEL avec vos assets (Tiles ou AutoLayer)
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

            // On crée un nouveau calque temporaire pour y mettre les tuiles de ce calque précisément
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

            // On ajoute ce calque à notre liste de calques
            m_layerTiles.push_back(currentLayerTiles);
        }
    }
    return true;
}

void Map::draw(sf::RenderWindow& window) {
    // LDtk enregistre les calques du HAUT vers le BAS dans le JSON.
    // Pour l'affichage, on doit faire l'INVERSE : dessiner du BAS (le sol) vers le HAUT (les arbres).
    // On parcourt donc notre tableau m_layerTiles à l'envers (du dernier élément au premier).
    for (int i = static_cast<int>(m_layerTiles.size()) - 1; i >= 0; --i) {
        
        // On dessine toutes les tuiles du calque courant 'i'
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
        // Correction ici : on utilise le nom déclaré dans le .hpp
        return m_grid[y][x] == VAL_CHEMIN; 
    }
    return false;
}

bool Map::isTowerZone(int x, int y) const {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        // Correction ici : on utilise le nom déclaré dans le .hpp
        return m_grid[y][x] == VAL_TOURS;
    }
    return false;
}

std::vector<sf::Vector2i> Map::genererChemin(sf::Vector2i pointDepart) {
    std::vector<sf::Vector2i> chemin;
    sf::Vector2i caseActuelle = pointDepart;
    chemin.push_back(caseActuelle);

    // Liste des 4 directions autour d'une case (Droite, Gauche, Bas, Haut)
    std::vector<sf::Vector2i> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    
    bool cheminEnCours = true;
    while (cheminEnCours) {
        bool caseTrouvee = false;

        for (const auto& dir : directions) {
            sf::Vector2i voisine = caseActuelle + dir;

            // On vérifie si la case voisine est un chemin
            if (isMonsterPath(voisine.x, voisine.y)) {
                // Pour ne pas faire demi-tour, on vérifie si elle n'est pas déjà dans notre chemin
                if (chemin.size() < 2 || voisine != chemin[chemin.size() - 2]) {
                    caseActuelle = voisine;
                    chemin.push_back(caseActuelle);
                    caseTrouvee = true;
                    break;
                }
            }
        }

        // Si aucune case "chemin" non visitée n'est autour, la route est finie (base atteinte)
        if (!caseTrouvee) {
            cheminEnCours = false;
        }
    }

    return chemin;
}


int Map::getWidth() const { return m_width; }
int Map::getHeight() const { return m_height; }