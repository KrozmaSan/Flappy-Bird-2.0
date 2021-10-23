#include "Level.h"

std::unique_ptr<Text> Level::m_WhiteNumbers[Level::SCORE_CAP];
std::unique_ptr<Text> Level::m_GoldenNumbers[Level::SCORE_CAP];
std::unique_ptr<Font> Level::m_ScoreFont(nullptr);

Level::Level(WindowInstance& app, TextureAtlas& environment, TextureAtlas& skins, const ScreenSide screenSide, int& keepMenuScroll, PlayerData& playerData)
:
    m_AppBind(app),
    m_ScreenSide(screenSide),
    m_Position(SDL_extensions::rectCreator((screenSide == ScreenSides::RIGHT) ? app.getWindowWidth() / 2 : 0, 0, (screenSide == ScreenSides::ALL) ? app.getWindowWidth() : app.getWindowWidth() / 2, app.getWindowHeight())),
    m_PlayerData(playerData),
    m_Background(app, playerData.getBackgroundPath(), m_Position),
    m_Floor(app, environment, screenSide),
    m_PipeManager(app, environment, m_Floor, screenSide),
    m_Player(app, skins, playerData.getSelectedSkin()),
    m_KeepMenuScroll(keepMenuScroll),
    m_Score(0)
{
    switch(screenSide)
    {
    case ScreenSides::LEFT:
        m_Player.setPosition(m_AppBind.getWindowWidth() / 4 - m_Player.getScreenPosition().w / 2, m_AppBind.getWindowHeight() / 2 - m_Player.getScreenPosition().h / 2);
        break;
    case ScreenSides::RIGHT:
        m_Player.setPosition(3 * m_AppBind.getWindowWidth() / 4 - m_Player.getScreenPosition().w / 2, m_AppBind.getWindowHeight() / 2 - m_Player.getScreenPosition().h / 2);
        break;
    case ScreenSides::ALL:default:
        m_Player.setPosition(Position::CENTER);
        break;
    }

    m_Background.setScroll(m_KeepMenuScroll, 0);
}

void Level::loadNumberGraphics(WindowInstance& app, const char* fontPath)
{
    //Considering how slow this method is, adding the overhead of a font opening isn't really significant.

    /*
    With this change, the font below should always be valid. This isn't that significant, as I never intended
    to modify or read from these Texts (the entire reason why I load them here...) but you never know.
    */

    m_ScoreFont.reset(new Font(fontPath));
    for(int i = 0 ; i < Level::SCORE_CAP ; i++)
    {
        m_WhiteNumbers[i].reset(new Text(app, std::to_string(i), *m_ScoreFont, 50, Colors::WHITE, 2, TextQuality::HIGH, Position::TOP));
        m_GoldenNumbers[i].reset(new Text(app, std::to_string(i), *m_ScoreFont, 50, Colors::GOLD, 2, TextQuality::HIGH, Position::TOP));
    }
}

void Level::unloadNumberGraphics()
{
    /*
    This solution is pretty fucking stupid.
    But it works. Why ? Don't ask. But it does.


    EDIT : after some research, chances are the d-tors of these only ran after TTF unloaded, causing
    a segfault when calling TTF_CloseFont().
    */


    m_ScoreFont.reset(nullptr);
    for(int i = 0 ; i < Level::SCORE_CAP ; i++)
    {
        m_WhiteNumbers[i].reset(nullptr);
        m_GoldenNumbers[i].reset(nullptr);
    }
}

void Level::playerJump()
{
    m_Player.jump();
}

void Level::show() const
{
    m_AppBind.saveRendererSettings();

    m_Background.show();
    m_PipeManager.showPipes();
    m_Player.show();
    m_Floor.show();
    m_WhiteNumbers[m_Score]->setPosition(m_Position.x + m_Position.w / 2 - m_WhiteNumbers[m_Score]->getScreenPosition().w / 2, m_WhiteNumbers[m_Score]->getScreenPosition().y);
    m_WhiteNumbers[m_Score]->show();
    m_GoldenNumbers[m_PlayerData.getHighScore()]->setNextTo(*m_WhiteNumbers[m_Score], 0, 25);
    m_GoldenNumbers[m_PlayerData.getHighScore()]->setPosition(m_Position.x + m_Position.w / 2 - m_GoldenNumbers[m_PlayerData.getHighScore()]->getScreenPosition().w / 2, m_GoldenNumbers[m_PlayerData.getHighScore()]->getScreenPosition().y);
    m_GoldenNumbers[m_PlayerData.getHighScore()]->show();
    m_AppBind.setDrawColor(Colors::BLACK);
    SDL_RenderDrawRect(m_AppBind.getRenderer(), &m_Position);

    m_AppBind.restoreRendererSettings();
}

bool Level::update()
{
    m_Score += m_Player.update(m_Floor, m_PipeManager);
    m_Score = std::min(m_Score, Level::SCORE_CAP - 1);
    m_PlayerData.trySetHighScore(m_Score);

    if(!m_Player.hasHitSomething())
    {
        m_Background.scroll(1, 0);
        m_KeepMenuScroll = m_Background.getScroll().x;
        m_Floor.scroll(1);
        m_PipeManager.scrollPipes();
    }

    return !m_Player.isBuried(); //Returns whether or not the level is still active
}

void Level::reset()
{
    m_Floor.setScroll(0);
    m_Score = 0;
    m_PipeManager.reset();
    m_Player.reset();
    m_Player.setPosition(m_Position.x + m_Position.w / 2 - m_Player.getScreenPosition().w / 2, m_Position.h / 2 - m_Player.getScreenPosition().h);
}
