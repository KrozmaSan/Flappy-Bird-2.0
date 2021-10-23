#ifndef PIPE_H
#define PIPE_H

#include <SDL_classes.h>
#include <Player.h>
#include <Floor.h>

class Floor; //Don't you love circular dependencies ?
class Player;
class PipeManager;

class Pipe : public Sprite
{
    public:
        explicit Pipe(WindowInstance& app, TextureAtlas& atlas, Floor const& floor, const int lowerBound, const int upperBound);
        virtual ~Pipe();

        static int computePipeWidth(WindowInstance const& app) {return 0.025 * app.getWindowWidth();}

        bool playerHitPipe(Player const& player);
        bool playerGotPoints(Player const& player);

    protected:
        static constexpr SDL_Rect SHAFT_ATLAS_POSITION = {10, 1, 48, 1};
        static constexpr SDL_Rect TIP_ATLAS_POSITION = {10, 0, 58, 1};

        Floor const& m_Floor;
        //When windowWidth = 1920; we roughly get 50. That's what I needed, and it should scale.
        const int PIPE_WIDTH = computePipeWidth(m_AppBind);
        const int PIPE_HEIGHT;
        const int TIP_HEIGHT = 0.04 * PIPE_HEIGHT;
        const int TIP_WIDTH = 1.4 * PIPE_WIDTH;
        const int SHAFT_OFFSET = (TIP_WIDTH - PIPE_WIDTH) / 2;
        int m_BreakPoint;
        const int BREAKPOINT_HEIGHT = 0.2 * PIPE_HEIGHT;
        //Prevent openings from appearing in the higher or lower fifths
        const int BREAKPOINT_MIN = 0.2 * PIPE_HEIGHT;
        const int BREAKPOINT_MAX = 0.8 * PIPE_HEIGHT - BREAKPOINT_HEIGHT;
        int m_LowerBound;
        int m_UpperBound;

        SDL_Rect m_TopShaft;
        SDL_Rect m_TopTip;
        SDL_Rect m_BottomTip;
        SDL_Rect m_BottomShaft;

        bool m_GavePoints = false;

        using Sprite::show;
        void show();
        void updateRects();
        void setScroll(const int x);
        void scroll(const int x);
        bool hasReachedEnd() const;
        void warp(const int x);
        void generateBreakpoint();
        void reset();

        friend class PipeManager;

    private:
};

#endif // PIPE_H
