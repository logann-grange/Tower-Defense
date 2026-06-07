#include "../controller/menuManager.hpp"


menuManager::menuManager(const std::string& backgroundPath)
    : menu(backgroundPath), sound(10), state(1) {}


menuManager::menuManager(Menu menu)
    : menu(std::move(menu)), sound(10), state(1) {}