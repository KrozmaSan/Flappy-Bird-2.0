#include "PipeManager.h"

/*
To determine the number of pipes, note the following inequality must always hold true to avoid empty spaces :
windowWidth <= numberOfPipes * (pipeWidth + distanceBetweenPipes).
By some rearranging, you end up with :
windowWidth / (pipeWidth + distanceBetweenPipes) <= numberOfPipes.
Which yields :
numberOfPipes = std::ceil(windowWidth / (pipeWidth + distanceBetweenPipes)) + 1;
*/

PipeManager::PipeManager(WindowInstance& app, TextureAtlas& atlas, Floor const& floor, const ScreenSide screenSide)
:
    m_AppBind(app),
    DISTANCE_BETWEEN_PIPES(10 * Pipe::computePipeWidth(m_AppBind)),
    m_NumberOfPipes(1 + ((screenSide == ScreenSides::ALL) ? m_AppBind.getWindowWidth() : m_AppBind.getWindowWidth() / 2) / (Pipe::computePipeWidth(m_AppBind) + DISTANCE_BETWEEN_PIPES)),
    m_Pipes(static_cast<Pipe*>(malloc(m_NumberOfPipes * sizeof(Pipe)))),
    m_Side(screenSide),
    m_LowerBound((screenSide == ScreenSides::RIGHT) ? app.getWindowWidth() / 2 : 0),
    m_UpperBound((screenSide == ScreenSides::LEFT) ? app.getWindowWidth() / 2 : app.getWindowWidth()),
    m_PointSound("assets/sfx/point.wav")
{
    for(int i = 0 ; i < m_NumberOfPipes ; i++)
    {
        new(m_Pipes + i) Pipe(app, atlas, floor, m_LowerBound, m_UpperBound);
        (m_Pipes + i)->setScroll(m_UpperBound + i * DISTANCE_BETWEEN_PIPES);
    }
}

PipeManager::~PipeManager()
{
    for(int i = 0 ; i < m_NumberOfPipes ; i++)
    {
        (m_Pipes + i)->~Pipe();
    }

    free(m_Pipes);
}

void PipeManager::showPipes() const
{
    for(int i = 0 ; i < m_NumberOfPipes ; i++)
    {
        (m_Pipes + i)->show();
    }
}

void PipeManager::scrollPipes()
{
    for(int i = 0 ; i < m_NumberOfPipes ; i++)
    {
        (m_Pipes + i)->scroll(PIPE_SPEED);
        if((m_Pipes + i)->hasReachedEnd())
        {
            /*
            Move to the right of the previous pipe, with wrap-around : so the third pipe will move to the right of the second, and the
            zeroth to the right of the last
            */

            int prevPipe = (i == 0) ? m_NumberOfPipes - 1 : i - 1;
            (m_Pipes + i)->warp((m_Pipes + prevPipe)->getScreenPosition().x + (m_Pipes + prevPipe)->getScreenPosition().w + DISTANCE_BETWEEN_PIPES);
        }
    }
}

bool PipeManager::playerHitPipes(Player const& player) const
{
    for(int i = 0 ; i < m_NumberOfPipes ; i++)
    {
        if((m_Pipes + i)->playerHitPipe(player)){return true;}
    }
    return false;
}

int PipeManager::awardPoints(Player const& player) const
{
    for(int i = 0 ; i < m_NumberOfPipes ; i++)
    {
        if((m_Pipes + i)->playerGotPoints(player))
        {
            m_PointSound.play();
            return 1;
        }
    }
    return 0;
}

void PipeManager::reset()
{
    for(int i = 0 ; i < m_NumberOfPipes ; i++)
    {
        (m_Pipes + i)->setScroll(m_UpperBound + i * DISTANCE_BETWEEN_PIPES);
        (m_Pipes + i)->reset();
    }
}

/*
Dealing with placement new is some other level of weird.
*/
