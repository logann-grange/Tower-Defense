#include "./../include/controlManager.hpp"
#include <iostream>

ControlManager::ControlManager(Menu& menu) : menu(menu) {
    labelToAction = {
        {"JOUER",      Action::JOUER},
        {"SCORES",     Action::SCORES},
        {"PARAMETRES", Action::PARAMETRES},
        {"QUITTER",    Action::QUITTER}
    };
}

ControlManager::Action ControlManager::resolveLabel(const std::string& label) const {
    auto it = labelToAction.find(label);
    if (it != labelToAction.end())
        return it->second;
    return Action::NONE;
}

// À appeler dans la boucle d'événements (sf::Event::MouseButtonPressed)
ControlManager::Action ControlManager::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            return pollActions(window);
        }
    }
    return Action::NONE;
}

// Vérifie quel bouton est cliqué via isHover + état de la souris
ControlManager::Action ControlManager::pollActions(sf::RenderWindow& window) {
    for (auto& btn : menu.buttonList) {
        if (btn.clicOn(window)) {
            // On retrouve l'action via le texte du bouton
            // (accès via getter ou ami — voir note ci-dessous)
            // Ici on suppose que Bouton expose getText()
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