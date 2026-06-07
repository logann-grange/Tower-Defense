#pragma once
#include <memory>
#include "../mob/Monster.hpp"
#include "../mob/MonsterView.hpp"

struct MonstreInstance {
    std::shared_ptr<Monster> logique;
    std::unique_ptr<MonsterView> graphique;
    bool recompenseDonnee = false;
};
 