#ifndef FLOOR_H
#define FLOOR_H

#include <SDL_classes.h>
#include <../Constants.h>
#include <Player.h>

class Player; //Don't you love circular dependencies ?

/*
Because of the way the floor rendering works, DO NOT try to scroll it backwards. It WILL go bad.
*/

class Floor : public Sprite
{
    public:
        explicit Floor(WindowInstance& app, TextureAtlas& atlas, const ScreenSide type);
        virtual ~Floor();

        int getVerticalPosition() const {return m_AppBind.getWindowHeight() - FLOOR_HEIGHT;}
        int getHeight() const {return FLOOR_HEIGHT;}

        void scroll(const uint8_t val);
        void setScroll(const uint8_t val);
        bool playerHitFloor(Player const& player) const;

        void show() const;

    protected:
        uint8_t m_Scroll;
        const ScreenSide m_Type;
        static constexpr int FLOOR_SCALE_FACTOR = 3;
        static constexpr int FLOOR_HEIGHT = 100;
        static constexpr int GRASS_HEIGHT = 7;
        static constexpr int GRASS_WIDTH = 10;
        static constexpr int EFFECTIVE_GRASS_HEIGHT = GRASS_HEIGHT * FLOOR_SCALE_FACTOR;
        static constexpr int EFFECTIVE_GRASS_WIDTH = GRASS_WIDTH * FLOOR_SCALE_FACTOR;

    private:
};

#endif // FLOOR_H
