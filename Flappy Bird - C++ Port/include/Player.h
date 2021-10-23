#ifndef PLAYER_H
#define PLAYER_H

#include <SDL_classes.h>

#include "../Constants.h"
#include <Floor.h>
#include <PipeManager.h>

class Floor; //Don't you love circular dependencies ?
class Pipe;
class PipeManager;

class Player : public Sprite
{
    public:
        explicit Player(WindowInstance& app, TextureAtlas& atlas, SkinPositions selectedSkin);
        virtual ~Player();

        bool hasHitSomething() const {return !m_IsAlive;}
        bool hasHitFloor() const {return m_HasCrashed;}
        bool isBuried() const {return m_Buried;}

        void setSkin(SkinPositions skin);

        void reset();
        void show() const;
        int update(Floor const& floor, PipeManager const& PipeManager);
        void jump() {if(m_IsAlive){m_JumpCount = 0; m_Flap.play();}}

        static constexpr int PLAYER_SPRITE_WIDTH = 17;
        static constexpr int PLAYER_SPRITE_HEIGHT = 12;

    protected:
        int m_Rotation;
        int m_JumpCount;
        bool m_IsAlive;
        bool m_HasCrashed;
        bool m_Buried;
        Counter m_DeathClock;
        Sound m_Flap;
        Sound m_Crash;

        void checkForDeath(Floor const& floor, PipeManager const& pipeManager);

        //Unused function, just keeping it in case I need it
        int jumpEquation(const int val) const {return SQUARE_FACTOR * val * val + JUMP_HEIGHT * val;}

        static constexpr int GRAVITY = 4;

        static constexpr int DEFAULT_ROTATION = 120;
        static constexpr double SQUARE_FACTOR = -1;
        static constexpr int JUMP_HEIGHT = 20;

    private:
};

#endif // PLAYER_H
