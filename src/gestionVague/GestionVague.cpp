#include "GestionVague.hpp"

using json = nlohmann::json;

bool GestionVague::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << path << std::endl;
        return false;
    }

    json data;
    file >> data;
     m_vagues.clear();

    std::vector<std::pair<int, json>> vaguesTriees;
    for (auto& [key, val] : data.items()) {
        vaguesTriees.push_back({ val["id"].get<int>(), val });
    }
    std::sort(vaguesTriees.begin(), vaguesTriees.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });
 
    for (auto& [id, val] : vaguesTriees) {
        Vague v;
        v.id = id;
        for (auto& m : val["monstres"]) {
            MonstreASpawner mas;
            mas.type      = m["type"].get<std::string>();
            mas.spawnTime = m["spawnTime"].get<float>();
            v.monstres.push_back(mas);
        }
        m_vagues.push_back(v);
    }
 
    std::cout << "WaveManager : " << m_vagues.size() << " vagues chargees." << std::endl;
    return true;
}

void GestionVague::update(float deltaTime,
                         std::vector<MonstreInstance>& listeMonstres,
                         const std::vector<sf::Vector2i>& chemin) {
    if (toutesVaguesTerminees()) return;
 
    const Vague& vague = m_vagues[m_vagueActuelle];
 
    m_tempsEcoule += deltaTime;
 
    // Spawne tous les monstres dont le spawnTime est dépassé
    while (m_indexProchainMonstre < static_cast<int>(vague.monstres.size())) {
        const MonstreASpawner& suivant = vague.monstres[m_indexProchainMonstre];
 
        if (m_tempsEcoule >= suivant.spawnTime) {
            MonstreInstance instance;
            instance.logique   = creerLogique(suivant.type);
            instance.graphique = creerGraphique(suivant.type);
 
            if (instance.logique && instance.graphique) {
                instance.logique->spawn(chemin);
                listeMonstres.push_back(std::move(instance));
                std::cout << "Spawn : " << suivant.type
                          << " (t=" << m_tempsEcoule << "s)" << std::endl;
            }
            m_indexProchainMonstre++;
        } else {
            break; // Le prochain n'est pas encore prêt
        }
    }
}

bool GestionVague::vagueTerminee() const {
    if (toutesVaguesTerminees()) return true;
    const Vague& vague = m_vagues[m_vagueActuelle];
    return m_indexProchainMonstre >= static_cast<int>(vague.monstres.size());
}

bool GestionVague::toutesVaguesTerminees() const {
    return m_vagueActuelle >= static_cast<int>(m_vagues.size());
}

void GestionVague::passerVagueSuivante() {
    if (!vagueTerminee()) {
        std::cerr << "Erreur : Impossible de passer à la vague suivante, la vague actuelle n'est pas terminée." << std::endl;
        return;
    }
    if (toutesVaguesTerminees()) return;
    m_vagueActuelle++;
    m_indexProchainMonstre = 0;
    m_tempsEcoule = 0.f;
}


std::unique_ptr<Monster> GestionVague::creerLogique(const std::string& type) {
    if (type == "Skeleton") {
        return std::make_unique<Skeleton>();
    } else if (type == "SkeletonWarrior") {
        return std::make_unique<SkeletonWarrior>();
    } else if (type == "SkeletonMage") {
        return std::make_unique<SkeletonMage>();
    } else {
        std::cerr << "Erreur : Type de monstre inconnu '" << type << "'" << std::endl;
        return nullptr;
    }
}

std::unique_ptr<MonsterView> GestionVague::creerGraphique(const std::string& type) {
    if (type == "Skeleton") {
        return std::make_unique<SkeletonView>();
    } else if (type == "SkeletonWarrior") {
        return std::make_unique<SkeletonWarriorView>();
    } else if (type == "SkeletonMage") {
        return std::make_unique<SkeletonMageView>();
    } else {
        std::cerr << "Erreur : Type de monstre inconnu '" << type << "'" << std::endl;
        return nullptr;
    }
}