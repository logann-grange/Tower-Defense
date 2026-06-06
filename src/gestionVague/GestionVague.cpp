#include "GestionVague.hpp"
 
using json = nlohmann::json;
 
GestionVague::GestionVague() {
    // Enregistrement des factories — pour ajouter un monstre,
    // il suffit d'ajouter une ligne ici et créer sa factory dans MonstreFactory.hpp
    m_factories["Skeleton"]        = std::make_unique<SkeletonFactory>();
    m_factories["SkeletonWarrior"] = std::make_unique<SkeletonWarriorFactory>();
    m_factories["SkeletonMage"]    = std::make_unique<SkeletonMageFactory>();
}
 
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
 
    std::cout << "GestionVague : " << m_vagues.size() << " vagues chargees." << std::endl;
    return true;
}
 
void GestionVague::update(float deltaTime,
                          std::vector<MonstreInstance>& listeMonstres,
                          const std::vector<sf::Vector2i>& chemin) {
    if (toutesVaguesTerminees()) return;
 
    const Vague& vague = m_vagues[m_vagueActuelle];
    m_tempsEcoule += deltaTime;
 
    while (m_indexProchainMonstre < static_cast<int>(vague.monstres.size())) {
        const MonstreASpawner& suivant = vague.monstres[m_indexProchainMonstre];
 
        if (m_tempsEcoule >= suivant.spawnTime) {
            auto it = m_factories.find(suivant.type);
            if (it != m_factories.end()) {
                MonstreInstance instance;
                instance.logique   = it->second->creerLogique();
                instance.graphique = it->second->creerGraphique();
                instance.logique->spawn(chemin);
                listeMonstres.push_back(std::move(instance));
                std::cout << "Spawn : " << suivant.type
                          << " (t=" << m_tempsEcoule << "s)" << std::endl;
            } else {
                std::cerr << "GestionVague : type inconnu -> " << suivant.type << std::endl;
            }
            m_indexProchainMonstre++;
        } else {
            break;
        }
    }
}
 
bool GestionVague::vagueTerminee() const {
    if (toutesVaguesTerminees()) return true;
    return m_indexProchainMonstre >= static_cast<int>(m_vagues[m_vagueActuelle].monstres.size());
}
 
bool GestionVague::toutesVaguesTerminees() const {
    return m_vagueActuelle >= static_cast<int>(m_vagues.size());
}
 
void GestionVague::passerVagueSuivante() {
    if (!vagueTerminee()) {
        std::cerr << "Erreur : La vague actuelle n'est pas terminee." << std::endl;
        return;
    }
    if (toutesVaguesTerminees()) return;
    m_vagueActuelle++;
    m_indexProchainMonstre = 0;
    m_tempsEcoule = 0.f;
    std::cout << "=== Vague " << m_vagueActuelle + 1 << " ===" << std::endl;
}
 