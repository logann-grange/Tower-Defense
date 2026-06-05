#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

class ResourceManager {
public:
    // Le singleton
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    sf::Texture& getTexture(const std::string& path) {
        auto it = m_textures.find(path);
        if (it != m_textures.end()) {
            return it->second; // Déjà chargée, on retourne directement
        }

        // Première fois : on charge et on stocke
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            std::cerr << "ResourceManager : Impossible de charger " << path << std::endl;
        }
        std::cout << "ResourceManager : Chargement de " << path << std::endl;
        m_textures[path] = std::move(tex);
        return m_textures[path];
    }

private:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::map<std::string, sf::Texture> m_textures;
};