#include "Main menu.h"

Gamemodes mainMenu(WindowInstance& app, int& keepMenuScroll, PlayerData& playerData)
{
    bool isDeleting = false;

    Font unispaceBold("assets/Unispace Bold", FontStyle::UNDERLINE);

    Text header(app, "Flappy Bird ++", unispaceBold, 100, Colors::WHITE, 1, TextQuality::HIGH, Position::TOP_LEFT);

    unispaceBold.setSize(50);
    unispaceBold.setStyle(FontStyle::NORMAL);

    TextButton classic(app, Colors::TBLACK, Colors::TBLACK, "Classic", unispaceBold, 50, Colors::WHITE, 1, TextQuality::HIGH, Position::LEFT);
    classic.setNextTo(header, 0, 100);

    TextButton versus(app, Colors::TBLACK, Colors::TBLACK, "Versus", unispaceBold, 50, Colors::WHITE, 1, TextQuality::HIGH, Position::NONE);
    versus.setNextTo(classic, 0, 25);

    TextButton skins(app, Colors::TBLACK, Colors::TBLACK, "Skins", unispaceBold, 50, Colors::WHITE, 1, TextQuality::HIGH, Position::NONE);
    skins.setNextTo(versus, 0, 25);

    TextButton backdrops(app, Colors::TBLACK, Colors::TBLACK, "Backdrops", unispaceBold, 50, Colors::WHITE, 1, TextQuality::HIGH, Position::NONE);
    backdrops.setNextTo(skins, 0, 25);

    TextureAtlas uiComponents(app, "assets/uiComponents.png");

    GraphicButton exit(app, Colors::RED, Colors::TBLACK, uiComponents, SDL_extensions::rectCreator(0, 0, 64, 64));
    exit.setPosition(Position::TOP_RIGHT);

    GraphicButton trash(app, Colors::RED, Colors::TBLACK, uiComponents, SDL_extensions::rectCreator(64, 0, 64, 64));
    trash.setNextTo(exit, -20, 0);

    Text deletionConfirm(app, "Are you sure you want to delete your save file ?", unispaceBold, 30, Colors::WHITE, 1, TextQuality::HIGH, Position::RIGHT);
    Text irreversibleNotice(app, "This is irreversible.", unispaceBold, 30, Colors::WHITE, 1, TextQuality::HIGH, Position::RIGHT);
    deletionConfirm.setNextTo(header, 0, 200);
    deletionConfirm.setPosition(Position::RIGHT);
    irreversibleNotice.setNextTo(deletionConfirm, 0, 1);

    TextButton yes(app, Colors::BLANK, Colors::RED, "Yes", unispaceBold, 80, Colors::WHITE, 1, TextQuality::HIGH, Position::NONE);
    yes.setNextTo(irreversibleNotice, 0, 1);
    yes.setActivity(false);

    TextButton no(app, Colors::BLANK, Colors::GREEN, "No", unispaceBold, 80, Colors::WHITE, 1, TextQuality::HIGH, Position::NONE);
    no.setNextTo(irreversibleNotice, 0, 1);
    no.setPosition(irreversibleNotice.getScreenPosition().x + irreversibleNotice.getScreenPosition().w - no.getScreenPosition().w, no.getScreenPosition().y);
    no.setActivity(false);

    std::unique_ptr<ScrollablePicture> picture(new ScrollablePicture(app, playerData.getBackgroundPath(), app.getWindowRect()));
    picture->setScroll(keepMenuScroll, 0);

    Timer updateTime(FrameTimes::SIXTY);

    while(true)
    {
        SDL_Event e; SDL_zero(e);
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT){return Gamemodes::EXIT;}

            if(classic.clickedOn(e))
            {
                return Gamemodes::CLASSIC;
            }

            if(versus.clickedOn(e))
            {
                return Gamemodes::VERSUS;
            }

            if(skins.clickedOn(e))
            {
                return Gamemodes::SKINS;
            }

            if(backdrops.clickedOn(e))
            {
                return Gamemodes::BACKDROPS;
            }

            if(exit.clickedOn(e))
            {
                return Gamemodes::EXIT;
            }

            if(trash.clickedOn(e))
            {
                isDeleting = true;
                yes.setActivity(true);
                no.setActivity(true);
                classic.setActivity(false);
                versus.setActivity(false);
                skins.setActivity(false);
                backdrops.setActivity(false);
                trash.setActivity(false);
                exit.setActivity(false);
            }

            if(yes.clickedOn(e))
            {
                isDeleting = false;
                yes.setActivity(false);
                no.setActivity(false);
                playerData.clean();
                classic.setActivity(true);
                versus.setActivity(true);
                skins.setActivity(true);
                backdrops.setActivity(true);
                trash.setActivity(true);
                exit.setActivity(true);
                picture.reset(new ScrollablePicture(app, playerData.getBackgroundPath(), app.getWindowRect()));
            }

            if(no.clickedOn(e))
            {
                isDeleting = false;
                yes.setActivity(false);
                no.setActivity(false);
                classic.setActivity(true);
                versus.setActivity(true);
                skins.setActivity(true);
                backdrops.setActivity(true);
                trash.setActivity(true);
                exit.setActivity(true);
            }
        }

        if(updateTime)
        {
            picture->scroll(1, 0);
            keepMenuScroll = picture->getScroll().x;
        }

        app.clean();

        picture->show();
        header.show();
        classic.show();
        versus.show();
        skins.show();
        backdrops.show();
        trash.show();
        exit.show();

        if(isDeleting)
        {
            app.paint(Colors::colorPicker(0, 0, 0, 200));
            irreversibleNotice.show();
            deletionConfirm.show();
            yes.show();
            no.show();
        }

        app.show();

    }
}


