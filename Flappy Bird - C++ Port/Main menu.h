#ifndef MAIN_MENU_H_INCLUDED
#define MAIN_MENU_H_INCLUDED

#include <vector>

#include <SDL_classes.h>

#include "Constants.h"
#include "Saving.h"

Gamemodes mainMenu(WindowInstance& app, int& keepMenuScroll, PlayerData& playerData);

#endif // MAIN_MENU_H_INCLUDED
