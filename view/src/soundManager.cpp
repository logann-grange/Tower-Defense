#include "soundManager.hpp"

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

    if (!buffer.loadFromFile(file)) {
        cout << "Erreur de chargement du son : " << file << endl;
        return;
    }
    sound.emplace(buffer);
    sound->setVolume(currentVolume);  // applique le volume courant
    sound->play();
}


void SoundManager::setVolume(int volume) {
    currentVolume = volume * 10;  // on sauvegarde
    music.setVolume(currentVolume);
    if (sound.has_value())
        sound->setVolume(currentVolume);
}