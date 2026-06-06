#pragma once
#include <memory>
#include "../mob/Monster.hpp"
#include "../mob/MonsterView.hpp"
#include "../mob/logic/Skeleton.hpp"
#include "../mob/logic/SkeletonWarrior.hpp"
#include "../mob/logic/SkeletonMage.hpp"
#include "../mob/view/SkeletonView.hpp"
#include "../mob/view/SkeletonWarriorView.hpp"
#include "../mob/view/SkeletonMageView.hpp"
 
// Interface abstraite — une fabrique par type de monstre
class MonstreFactory {
public:
    virtual ~MonstreFactory() = default;
    virtual std::unique_ptr<Monster>     creerLogique()   = 0;
    virtual std::unique_ptr<MonsterView> creerGraphique() = 0;
};
 
class SkeletonFactory : public MonstreFactory {
public:
    std::unique_ptr<Monster>     creerLogique()   override { return std::make_unique<Skeleton>(); }
    std::unique_ptr<MonsterView> creerGraphique() override { return std::make_unique<SkeletonView>(); }
};
 
class SkeletonWarriorFactory : public MonstreFactory {
public:
    std::unique_ptr<Monster>     creerLogique()   override { return std::make_unique<SkeletonWarrior>(); }
    std::unique_ptr<MonsterView> creerGraphique() override { return std::make_unique<SkeletonWarriorView>(); }
};
 
class SkeletonMageFactory : public MonstreFactory {
public:
    std::unique_ptr<Monster>     creerLogique()   override { return std::make_unique<SkeletonMage>(); }
    std::unique_ptr<MonsterView> creerGraphique() override { return std::make_unique<SkeletonMageView>(); }
};
 