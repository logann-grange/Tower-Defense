#include "../ui/soundManager.hpp"

SoundManager::SoundManager() {}

void SoundManager::playMusic(const string& gameState) {
    string file = "";
    if (gameState == "menu")
        file = "assets/sounds/music_menu.wav";
    else if (gameState == "game")
        file = "assets/sounds/music_game.wav";

    if (!music.openFromFile(file)) { //chargement de la musique
        cout << "Erreur de chargement de la musique : " << file << endl;
        return;
    }
    music.setLooping(true);
    music.play();
}

void SoundManager::stopMusic() {
    music.stop();
}

void SoundManager::playButtonSound(const string& buttonName) {
    string file = "";
    if (buttonName == "JOUER")           file = "assets/sounds/son_bouton.wav";
    else if (buttonName == "SCORES")     file = "assets/sounds/son_retour.mp3";
    else if (buttonName == "PARAMETRES") file = "assets/sounds/son_bouton.wav";
    else if (buttonName == "QUITTER")    file = "assets/sounds/son_bouton.wav";
    else if (buttonName == "RETOUR")     file = "assets/sounds/son_retour.mp3";
    else if (buttonName == "VOLUMEPLUS" || buttonName == "VOLUMEMOINS") file = "assets/sounds/son_bouton.wav";
    else return;

    if (!m_buttonBuffer.loadFromFile(file)) {
        cout << "Erreur son bouton : " << file << endl;
        return;
    }
    m_buttonSound.emplace(m_buttonBuffer);
    m_buttonSound->setVolume(currentVolume);
    m_buttonSound->play();
}

void SoundManager::playProjectileSound(const std::string& type, int niveau) {
    std::string file = "";
    if (type == "Feu")   file = "assets/sounds/tir_feu.wav";
    else if (type == "Glace") file = "assets/sounds/tir_glace.wav";

    if (file.empty()) return;

    if (!m_projectileBuffer.loadFromFile(file)) {
        std::cout << "Erreur son projectile : " << file << std::endl;
        return;
    }
    m_projectileSound.emplace(m_projectileBuffer);
    m_projectileSound->setVolume(currentVolume);
    m_projectileSound->play();
}

void SoundManager::setVolume(int volume) {
    currentVolume = volume * 10;
    music.setVolume(currentVolume);
    if (m_buttonSound.has_value())     m_buttonSound->setVolume(currentVolume);
    if (m_projectileSound.has_value()) m_projectileSound->setVolume(currentVolume);
}