#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
#include <string>
#include "./../../view/include/menu.hpp"

class ControlManager {
public:
    // Actions possibles depuis le menu
    enum class Action {
        NONE,
        JOUER,
        SCORES,
        PARAMETRES,
        QUITTER
    };

    explicit ControlManager(Menu& menu);

    // Retourne l'action déclenchée par un clic, ou Action::NONE
    Action handleEvent(sf::RenderWindow& window, const sf::Event& event);

    // Surcharge pour polling (sans événement explicite)
    Action pollActions(sf::RenderWindow& window);

    // Enregistre un callback personnalisé pour une action
    void bindAction(Action action, std::function<void()> callback);

    // Exécute le callback lié à l'action
    void execute(Action action);

private:
    Menu& menu;
    std::unordered_map<std::string, Action> labelToAction;
    std::unordered_map<int, std::function<void()>> callbacks;

    Action resolveLabel(const std::string& label) const;
};