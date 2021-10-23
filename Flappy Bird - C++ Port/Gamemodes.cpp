#include "Gamemodes.h"

/*
After some investigation, it turns out the occasional slowdowns aren't caused by logic or drawing sprites, but by the GPU's rendering, i.e.
calls to SDL_RenderPresent() (i.e. GPU slowdowns). There's not much I can do about it, so I guess it'll stay this way...
*/


/**
Some might say this is awful design, as this strongly looks like 2 times the same function with minimal changes.
And they'd be right...to some extent. Even though the core loop is the exact same, some things subtly change,
so it's actually easier to split them in two and fine-tune everything than figuring out out to modularize
everything with function pointers and all.
*/


bool classic(WindowInstance& app, int& keepMenuScroll, PlayerData& playerData)
{
    KeyboardHandler keyboard;
    Timer updateTime(FrameTimes::SIXTY);
    TextureAtlas skins(app, "assets/player.png");
    TextureAtlas environment(app, "assets/environment.png");
    TextureAtlas ui(app, "assets/uiComponents.png");

    Font unispaceBold("assets/Unispace Bold.ttf");

    Level level(app, environment, skins, ScreenSides::ALL, keepMenuScroll, playerData);

    GraphicButton backToMenu(app, Colors::RED, Colors::TBLACK, ui, {64, 64, 64, 64});
    GraphicButton replay(app, Colors::GREEN, Colors::TBLACK, ui, {0, 64, 64, 64});
    backToMenu.setPosition(Position::CENTER); backToMenu.addPosition(2 * backToMenu.getScreenPosition().w, 0);
    replay.setPosition(Position::CENTER); replay.addPosition((-2) * replay.getScreenPosition().w, 0);

    Text getReady(app, "Press Space to begin !", unispaceBold, 50, Colors::WHITE, 2, TextQuality::HIGH, Position::CENTER);
    Text gameOver(app, "GAME OVER", unispaceBold, 100, Colors::WHITE, 3, TextQuality::HIGH, Position::HORIZONTAL);
    gameOver.setNextTo(backToMenu, 0, -50);
    gameOver.setPosition(Position::HORIZONTAL);

    bool r = true;
    bool playing = true;
    bool readyUp = true;
    while(r)
    {
        while(readyUp)
        {
            SDL_Event e; SDL_zero(e);
            while(SDL_PollEvent(&e))
            {
                keyboard.processKeyPresses(e);
            }
            if(keyboard.keyPressed(Keycodes::SPACEBAR))
            {
                readyUp = false;
            }
            app.clean();
            level.show();
            app.paint(Colors::TBLACK);
            getReady.show();
            app.show();
        }

        SDL_Event e; SDL_zero(e);
        if(playing)
        {
            backToMenu.setVisibility(false);
            replay.setVisibility(false);
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT){return false;}
                keyboard.processKeyPresses(e);
            }

            if(keyboard.keyPressed(Keycodes::SPACEBAR))
            {
                level.playerJump();
            }

            keyboard.processKeyHelds();

            if(updateTime)
            {
                playing = level.update();
            }
        }

        app.clean();
        level.show();

        if(!playing)
        {
            app.paint(Colors::TBLACK);
            backToMenu.setVisibility(true);
            replay.setVisibility(true);
            backToMenu.show();
            replay.show();
            gameOver.show();
            while(SDL_PollEvent(&e))
            {
                keyboard.processKeyPresses(e);
                if(backToMenu.clickedOn(e))
                {
                    r = false;
                }
                if(replay.clickedOn(e) || keyboard.keyPressed(Keycodes::SPACEBAR))
                {
                    playing = true;
                    readyUp = true;
                    level.reset();
                }
            }
        }

        app.show();
    }

    return true;
}

bool versus(WindowInstance& app, int& keepMenuScroll, PlayerData& playerData)
{
    KeyboardHandler keyboard;
    Timer updateTime(FrameTimes::SIXTY);
    TextureAtlas skins(app, "assets/player.png");
    TextureAtlas environment(app, "assets/environment.png");
    TextureAtlas ui(app, "assets/uiComponents.png");

    Font unispaceBold("assets/Unispace Bold.ttf");

    Level leftSide(app, environment, skins, ScreenSides::LEFT, keepMenuScroll, playerData);
    Level rightSide(app, environment, skins, ScreenSides::RIGHT, keepMenuScroll, playerData);

    GraphicButton backToMenu(app, Colors::RED, Colors::TBLACK, ui, {64, 64, 64, 64});
    GraphicButton replay(app, Colors::GREEN, Colors::TBLACK, ui, {0, 64, 64, 64});
    backToMenu.setPosition(Position::CENTER); backToMenu.addPosition(2 * backToMenu.getScreenPosition().w, 0);
    replay.setPosition(Position::CENTER); replay.addPosition((-2) * replay.getScreenPosition().w, 0);

    Text getReady(app, "Press Space and Up to begin !", unispaceBold, 50, Colors::WHITE, 2, TextQuality::HIGH, Position::CENTER);
    Text gameOver(app, "GAME OVER", unispaceBold, 100, Colors::WHITE, 3, TextQuality::HIGH, Position::HORIZONTAL);
    gameOver.setNextTo(backToMenu, 0, -50);
    gameOver.setPosition(Position::HORIZONTAL);

    bool r = true;
    bool playing = true;
    bool readyUp = true;
    while(r)
    {
        while(readyUp)
        {
            SDL_Event e; SDL_zero(e);
            while(SDL_PollEvent(&e))
            {
                keyboard.processKeyPresses(e);
            }
            keyboard.processKeyHelds();
            if(keyboard.keyHeld(Keycodes::SPACEBAR) && keyboard.keyHeld(Keycodes::ARROW_UP))
            {
                readyUp = false;
            }

            app.clean();
            leftSide.show();
            rightSide.show();

            app.paint(Colors::TBLACK);
            getReady.show();
            app.show();
        }

        SDL_Event e; SDL_zero(e);
        if(playing)
        {
            backToMenu.setVisibility(false);
            replay.setVisibility(false);
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT){return false;}
                keyboard.processKeyPresses(e);
            }

            if(keyboard.keyPressed(Keycodes::SPACEBAR))
            {
                leftSide.playerJump();
            }

            if(keyboard.keyPressed(Keycodes::ARROW_UP))
            {
                rightSide.playerJump();
            }

            keyboard.processKeyHelds();

            if(updateTime)
            {
                /*
                Gotta do this ridiculousness because of short-circuit evaluation.
                */
                bool a = leftSide.update();
                bool b = rightSide.update();
                playing = a || b;
            }
        }

        app.clean();

        leftSide.show();
        rightSide.show();

        if(!playing)
        {
            app.paint(Colors::TBLACK);
            backToMenu.setVisibility(true);
            replay.setVisibility(true);
            backToMenu.show();
            replay.show();
            gameOver.show();
            while(SDL_PollEvent(&e))
            {
                keyboard.processKeyPresses(e);
                if(backToMenu.clickedOn(e))
                {
                    r = false;
                }
                if(replay.clickedOn(e) || (keyboard.keyPressed(Keycodes::SPACEBAR) && keyboard.keyPressed(Keycodes::ARROW_UP)))
                {
                    playing = true;
                    readyUp = true;
                    leftSide.reset();
                    rightSide.reset();
                }
            }
        }

        app.show();
    }

    return true;
}

