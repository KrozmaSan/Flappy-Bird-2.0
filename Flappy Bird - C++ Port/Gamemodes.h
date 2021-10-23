#ifndef GAMEMODES_H_INCLUDED
#define GAMEMODES_H_INCLUDED

#include <SDL_classes.h>
#include "Level.h"

bool classic(WindowInstance& app, int& keepMenuScroll, PlayerData& playerData);
bool versus(WindowInstance& app, int& keepMenuScroll, PlayerData& playerData);

#endif // GAMEMODES_H_INCLUDED
