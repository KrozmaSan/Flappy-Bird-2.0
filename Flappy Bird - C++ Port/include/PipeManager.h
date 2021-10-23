#ifndef PIPEMANAGER_H
#define PIPEMANAGER_H

#include "../Constants.h"
#include "Pipe.h"
#include <cmath>

class Pipe;

class PipeManager
{
    public:
        explicit PipeManager(WindowInstance& app, TextureAtlas& atlas, Floor const& floor, const ScreenSide screenSide);
        virtual ~PipeManager();

        void reset();
        void scrollPipes();
        void showPipes() const;
        bool playerHitPipes(Player const& player) const;
        int awardPoints(Player const& player) const;

    protected:
        WindowInstance& m_AppBind;
        const int DISTANCE_BETWEEN_PIPES;
        const int m_NumberOfPipes;
        Pipe* m_Pipes;
        ScreenSide m_Side;
        const int m_LowerBound;
        const int m_UpperBound;
        Sound m_PointSound;

        static constexpr int PIPE_SPEED = -5;

    private:
};

#endif // PIPEMANAGER_H
