#include "BackdropSelector.h"

bool backdropSelector(WindowInstance& app, PlayerData& playerData)
{
    Font unispaceBold("assets/Unispace Bold");
    BackgroundManager bgMan(app, playerData, "assets/backgrounds/backgroundStrings.data", unispaceBold);

    TextureAtlas uiComponents(app, "assets/uiComponents.png");
    GraphicButton exit(app, Colors::RED, Colors::TBLACK, uiComponents, SDL_extensions::rectCreator(64, 64, 64, 64));
    exit.setPosition(Position::TOP_RIGHT);
    GraphicButton left(app, Colors::BLANK, Colors::TBLACK, uiComponents, SDL_extensions::rectCreator(0, 128, 64, 64));
    left.setPosition(Position::VERTICAL|Position::LEFT);
    GraphicButton right(app, Colors::BLANK, Colors::TBLACK, uiComponents, SDL_extensions::rectCreator(64, 128, 64, 64));
    right.setPosition(Position::VERTICAL|Position::RIGHT);

    KeyboardHandler k;

    while(true)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT){return false;}
            k.processKeyPresses(e);
            if(exit.clickedOn(e)){return true;}
            if(k.keyPressed(Keycodes::ARROW_LEFT) || left.clickedOn(e)){bgMan.scrollLeft();}
            if(k.keyPressed(Keycodes::ARROW_RIGHT) || right.clickedOn(e)){bgMan.scrollRight();}
            if(bgMan.requestingSelect(e)){bgMan.trySelect();}
            if(e.type == SDL_MOUSEWHEEL)
            {
                if(e.wheel.y > 0)
                {
                    bgMan.scrollLeft();
                }
                else
                {
                    bgMan.scrollRight();
                }
            }
        }

        bgMan.show();
        exit.show();
        left.show();
        right.show();
        app.show();
    }
}

unsigned int determineAmountOfBackgrounds()
{
    for(unsigned int i = 0 ; ; i++)
    {
        std::string foo = "assets/backgrounds/bg" + std::to_string(i) + ".png";
        if(!myUtilities::fileExists(foo))
        {
            return i;
        }
    }
}
