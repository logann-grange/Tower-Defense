#include "../menu/menu.hpp"

class menuManager {
    public : 
        int state;
        int sound;
        Menu menu;

        menuManager() = delete; // ← interdit le constructeur sans argument
        menuManager(const std::string& backgroundPath);
        menuManager(Menu menu);
        bool runMenu();

};