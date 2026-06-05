#ifndef SKELETON_VIEW_HPP
#define SKELETON_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../MonsterView.hpp"     // On remonte pour chercher l'interface de vue
#include "../logic/Skeleton.hpp" // On remonte, puis on va dans logic
#include "../../ressource/RessourceManager.hpp"


class SkeletonView : public MonsterView {
public:
    SkeletonView();
    
    // Le mot-clé "override" confirme qu'on applique le contrat de MonsterView
    void update(float deltaTime, const Monster& logique) override;
    void draw(sf::RenderWindow& window) override;

    bool estPretADetruire() const override { return m_animationMortTermine; }

private:
    // SFML 3 : Les textures d'abord, le sprite ensuite !
    //sf::Texture m_textureDroite;
    //sf::Texture m_textureGauche;
    //sf::Texture m_textureMort;
    const sf::Texture* m_pTextureDroite = nullptr;
    const sf::Texture* m_pTextureGauche = nullptr;
    const sf::Texture* m_pTextureMort   = nullptr;
    sf::Sprite m_sprite; 
    sf::RectangleShape m_barreFond;
    sf::RectangleShape m_barrePV;

    bool m_animationMortTermine = false;


    // Variables de découpe pour tes images en 384x64 (6 frames de 64x64)
    int m_frameLargeur = 64;
    int m_frameHauteur = 64;
    int m_nbFramesMax = 6;
    int m_frameActuelle = 0;
    
    float m_tempsAnimation = 0.f;
    float m_vitesseAnimation = 0.1f; // 100ms par image
};

#endif