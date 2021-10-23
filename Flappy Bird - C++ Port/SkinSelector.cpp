#include "SkinSelector.h"

bool skinSelector(WindowInstance& app, int& keepMenuScroll, PlayerData& playerData)
{
    KeyboardHandler keyboard;
    ScrollablePicture background(app, playerData.getBackgroundPath(), app.getWindowRect());
    background.setScroll(keepMenuScroll, background.getScroll().y);

    TextureAtlas uiComponents(app, "assets/uiComponents.png");
    GraphicButton exit(app, Colors::RED, Colors::TBLACK, uiComponents, SDL_extensions::rectCreator(64, 64, 64, 64));
    exit.setPosition(Position::TOP_RIGHT);
    GraphicButton left(app, Colors::BLANK, Colors::TBLACK, uiComponents, SDL_extensions::rectCreator(0, 128, 64, 64));
    left.setPosition(Position::VERTICAL|Position::LEFT);
    GraphicButton right(app, Colors::BLANK, Colors::TBLACK, uiComponents, SDL_extensions::rectCreator(64, 128, 64, 64));
    right.setPosition(Position::VERTICAL|Position::RIGHT);

    Timer updateTime(FrameTimes::SIXTY);

    SelectManager selector(app, "assets/player.png", "cosmeticStrings.data", "assets/Unispace Bold", playerData);

    while(true)
    {
        SDL_Event e; SDL_zero(e);
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT){return false;}
            keyboard.processKeyPresses(e);
            if(exit.clickedOn(e)){return true;}
            if(keyboard.keyPressed(Keycodes::ARROW_LEFT) || left.clickedOn(e)){selector.scrollLeft();}
            if(keyboard.keyPressed(Keycodes::ARROW_RIGHT) || right.clickedOn(e)){selector.scrollRight();}
            if(selector.requestingSelect(e)){selector.setSelection();}
            if(e.type == SDL_MOUSEWHEEL)
            {
                if(e.wheel.y > 0)
                {
                    selector.scrollLeft();
                }
                else
                {
                    selector.scrollRight();
                }
            }
        }

        if(updateTime)
        {
            keepMenuScroll++;
            background.scroll(1, 0);
        }

        app.clean();

        background.show();
        selector.show();

        exit.show();
        left.show();
        right.show();

        app.show();
    }
}
