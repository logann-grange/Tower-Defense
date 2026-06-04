#ifndef SKELETON_WARRIOR_VIEW_HPP
#define SKELETON_WARRIOR_VIEW_HPP


#include <SFML/Graphics.hpp>
#include <iostream>
#include "../MonsterView.hpp"     // On remonte pour chercher l'interface de vue
#include "../logic/SkeletonWarrior.hpp" 

class SkeletonWarriorView : public MonsterView {
    public:
    SkeletonWarriorView();
    
    // Le mot-clé "override" confirme qu'on applique le contrat de MonsterView
    void update(float deltaTime, const Monster& logique) override;
    void draw(sf::RenderWindow& window) override;

    bool estPretADetruire() const override { return m_animationMortTermine; }

private:
    // SFML 3 : Les textures d'abord, le sprite ensuite !
    sf::Texture m_textureDroite;
    sf::Texture m_textureGauche;
    sf::Texture m_textureMort;
    sf::Sprite m_sprite; 
    sf::RectangleShape m_barreFond;
    sf::RectangleShape m_barrePV;

    bool m_animationMortTermine = false;


    // Variables de découpe pour tes images en 384x64 (6 frames de 64x64)
    int m_frameLargeur = 64;
    int m_frameHauteur = 64;
    int mframeHauteurMort = 48; // ← adapte à la nouvelle hauteur de frame
    int m_nbFramesMax = 6;
    int m_frameActuelle = 0;
    
    float m_tempsAnimation = 0.f;
    float m_vitesseAnimation = 0.1f; // 100ms par image
};


#endif // SKELETON_WARRIOR_VIEW_HPP