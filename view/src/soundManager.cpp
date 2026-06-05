#include "soundManager.hpp"

SoundManager::SoundManager() {}

void SoundManager::playMusic(const string& gameState) {
    string file = "";
    if (gameState == "menu")
        file = "assets/sounds/music_menu.wav";
    else if (gameState == "game")
        file = "assets/sounds/music_game.wav";

    if (!music.openFromFile(file)) {  // méthode d'instance, pas statique
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

    if (!buffer.loadFromFile(file)) {  // méthode d'instance
        cout << "Erreur de chargement du son : " << file << endl;
        return;
    }
    sound.emplace(buffer);  // on crée le Sound avec le buffer
    sound->play();
}