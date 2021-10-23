#ifndef BACKDROPSELECTOR_H_INCLUDED
#define BACKDROPSELECTOR_H_INCLUDED

#include <SDL_classes.h>
#include "Saving.h"
#include "../Constants.h"
#include "BackgroundManager.h"

bool backdropSelector(WindowInstance& app, PlayerData& playerData);
unsigned int determineAmountOfBackgrounds();

#endif // BACKDROPSELECTOR_H_INCLUDED
