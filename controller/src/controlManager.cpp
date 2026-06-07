#include "controlManager.hpp"
#include <iostream>

ControlManager::ControlManager(std::vector<Bouton>& initialButtons)
    : activeButtons(&initialButtons)
{
    labelToAction = {
        {"JOUER",        Action::JOUER},
        {"SCORES",       Action::SCORES},
        {"PARAMETRES",   Action::PARAMETRES},
        {"QUITTER",      Action::QUITTER},
        {"RETOUR",       Action::RETOUR},
        {"VOLUMEPLUS",   Action::VOLUMEPLUS},
        {"VOLUMEMOINS",  Action::VOLUMEMOINS},
    };
}

void ControlManager::setActiveButtons(std::vector<Bouton>& buttons) {
    activeButtons = &buttons;
}

ControlManager::Action ControlManager::resolveLabel(const std::string& label) const {
    auto it = labelToAction.find(label);
    if (it != labelToAction.end())
        return it->second;
    return Action::NONE;
}

ControlManager::Action ControlManager::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            return pollActions(window);
        }
    }
    return Action::NONE;
}

ControlManager::Action ControlManager::pollActions(sf::RenderWindow& window) {
    for (auto& btn : *activeButtons) {  
        if (btn.clicOn(window)) {
            Action action = resolveLabel(btn.text);
            execute(action);
            return action;
        }
    }
    return Action::NONE;
}

void ControlManager::bindAction(Action action, std::function<void()> callback) {
    callbacks[static_cast<int>(action)] = std::move(callback);
}

void ControlManager::execute(Action action) {
    if (action == Action::NONE) return;
    auto it = callbacks.find(static_cast<int>(action));
    if (it != callbacks.end()) {
        it->second();
    } else {
        std::cout << "[ControlManager] Aucun callback pour l'action "
                  << static_cast<int>(action) << std::endl;
    }
}