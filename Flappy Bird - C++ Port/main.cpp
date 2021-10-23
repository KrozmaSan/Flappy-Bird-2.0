#define SDL_MAIN_HANDLED

#include <SDL_classes.h>

#include "icon.h"

#include "Constants.h"
#include "Main menu.h"
#include "Gamemodes.h"
#include "SkinSelector.h"
#include "BackdropSelector.h"

/**
Indeed, the project changed quite a bit since 1.3.
Why you ask?
Because I deleted every Python-related part from it, considering it would never see the light of day.
How great to be given up upon in the middle of a project.
That considerably cleaned up things. This, combined with deleting all graphics-related code and porting it to SDL_classes.a,
made the project much tidier.
I also reworked saves.
*/

int main()
{
    try{

    WindowInstance app("Flappy Bird ++", ICON_PATH, nullptr, HD_WIDTH, HD_HEIGHT, VSyncOptions::ENABLE, WindowOptions::FULLSCREEN);
    AbstractButton::setGlobalClickSound("assets/sfx/click.wav");
    int keepMenuScroll = 0;
    PlayerData playerData("saveFile.data");

    Level::loadNumberGraphics(app, "assets/Unispace Bold");

    bool r = true;

    while(r)
    {
        Gamemodes choice = mainMenu(app, keepMenuScroll, playerData);
        playerData.load(); //In case the menu messed with it, reload it
        switch(choice)
        {
        case Gamemodes::EXIT:
            r = false;
            break;
        case Gamemodes::CLASSIC:
            r = classic(app, keepMenuScroll, playerData);
            break;
        case Gamemodes::VERSUS:
            r = versus(app, keepMenuScroll, playerData);
            break;
        case Gamemodes::SKINS:
            r = skinSelector(app, keepMenuScroll, playerData);
            break;
        case Gamemodes::BACKDROPS:
            r = backdropSelector(app, playerData);
            break;
        case Gamemodes::INVALID:
            throw Gamemodes::INVALID;
            break;
        default:break;
        }
        playerData.save();
    }

    Level::unloadNumberGraphics();
    return EXIT_SUCCESS;

    }

    catch(enum Gamemodes)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error !", "Uncaught exception during menu execution. Aborting execution.", NULL);
        return EXIT_FAILURE;
    }

    catch(std::exception& e)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error !", "Uncaught exception. Aborting execution.", NULL);
        myUtilities::emergencyLog(e.what());
        return EXIT_FAILURE;
    }

    catch(...)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error !", "Unexpected crash. Aborting execution.", NULL);
        return EXIT_FAILURE;
    }
}
