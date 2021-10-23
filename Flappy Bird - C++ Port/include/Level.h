#ifndef LEVEL_H
#define LEVEL_H

#include <SDL_classes.h>

#include "Floor.h"
#include "PipeManager.h"
#include "Player.h"
#include "../Constants.h"
#include "../Saving.h"

class Level
{
    public:
        explicit Level(WindowInstance& app, TextureAtlas& environment, TextureAtlas& skins, const ScreenSide screenSide, int& keepMenuScroll, PlayerData& playerData);
        virtual ~Level() noexcept {}

        SDL_Rect getPosition() const {return m_Position;}
        int getScore() const {return m_Score;}

        bool update();
        void playerJump();
        void show() const;
        void reset();

        static constexpr int SCORE_CAP = 1000;

        static void loadNumberGraphics(WindowInstance& app, const char* fontPath);
        static void loadNumberGraphics(WindowInstance& app, std::string const& fontPath){loadNumberGraphics(app, fontPath.c_str());}
        static void unloadNumberGraphics();

    protected:
        WindowInstance& m_AppBind;
        const ScreenSide m_ScreenSide;
        const SDL_Rect m_Position;
        PlayerData& m_PlayerData;
        ScrollablePicture m_Background;
        Floor m_Floor;
        PipeManager m_PipeManager;
        Player m_Player;
        int& m_KeepMenuScroll;
        int m_Score;
        static std::unique_ptr<Font> m_ScoreFont;
        static std::unique_ptr<Text> m_WhiteNumbers[Level::SCORE_CAP];
        static std::unique_ptr<Text> m_GoldenNumbers[Level::SCORE_CAP];

    private:
};

#endif // LEVEL_H
