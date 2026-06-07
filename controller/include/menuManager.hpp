#include "menu.hpp"

class menuManager {
    public : 
        int state;
        int sound;
        Menu menu;

        menuManager();
        menuManager(Menu menu);
        bool runMenu();

};