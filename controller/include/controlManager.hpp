#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include "bouton.hpp"

class ControlManager {
public:
    enum class Action {
        NONE,
        JOUER,
        SCORES,
        PARAMETRES,
        QUITTER,
        RETOUR,
        VOLUMEPLUS,
        VOLUMEMOINS
    };

    explicit ControlManager(std::vector<Bouton>& initialButtons);

    void setActiveButtons(std::vector<Bouton>& buttons);

    Action handleEvent(sf::RenderWindow& window, const sf::Event& event);
    Action pollActions(sf::RenderWindow& window);

    void bindAction(Action action, std::function<void()> callback);
    void execute(Action action);

private:
    std::vector<Bouton>* activeButtons;
    std::unordered_map<std::string, Action> labelToAction;
    std::unordered_map<int, std::function<void()>> callbacks;

    Action resolveLabel(const std::string& label) const;
};