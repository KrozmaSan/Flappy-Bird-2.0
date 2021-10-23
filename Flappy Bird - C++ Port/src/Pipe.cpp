#include "Pipe.h"

Pipe::Pipe(WindowInstance& app, TextureAtlas& atlas, Floor const& floor, const int lowerBound, const int upperBound)
:
    AbstractGraphics(app, Position::NONE),
    Sprite(app, atlas, SDL_VOID_RECT),
    m_Floor(floor),
    PIPE_HEIGHT(m_AppBind.getWindowHeight() - m_Floor.getHeight() + 1),
    m_LowerBound(lowerBound),
    m_UpperBound(upperBound)
{
    m_ScreenPosition = SDL_extensions::rectCreator(upperBound, 0, TIP_WIDTH, PIPE_HEIGHT);
    updateRects();
    generateBreakpoint();
}

Pipe::~Pipe()
{
    //dtor
}

void Pipe::generateBreakpoint()
{
    m_BreakPoint = BREAKPOINT_MIN + rand() % (BREAKPOINT_MAX - BREAKPOINT_MIN);
}

void Pipe::setScroll(const int x)
{
    m_ScreenPosition.x = x;
}

void Pipe::scroll(const int x)
{
    m_ScreenPosition.x += x;
}

bool Pipe::hasReachedEnd() const
{
    return (m_ScreenPosition.x + m_ScreenPosition.w < m_LowerBound);
}

void Pipe::warp(const int x)
{
    setScroll(x);
    generateBreakpoint();
    m_GavePoints = false;
}

void Pipe::show()
{
    if(!m_Displayed){return;}
    updateRects();
    m_AppBind.saveRendererSettings();

    //If nothing special needs to happen, just draw
    if(m_ScreenPosition.x >= m_LowerBound && m_ScreenPosition.x + m_ScreenPosition.w <= m_UpperBound)
    {
        SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &SHAFT_ATLAS_POSITION, &m_TopShaft);
        SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &SHAFT_ATLAS_POSITION, &m_BottomShaft);
        SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &TIP_ATLAS_POSITION, &m_BottomTip);
        SDL_RenderCopyEx(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &TIP_ATLAS_POSITION, &m_TopTip, 0, NULL, SDL_FLIP_VERTICAL);

        m_AppBind.setDrawColor(Colors::BLACK);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &m_TopShaft);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &m_TopTip);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &m_BottomTip);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &m_BottomShaft);
    }

    //First case : pipe is at the left of the screen
    else if(m_ScreenPosition.x < m_LowerBound && m_ScreenPosition.x + m_ScreenPosition.w > m_LowerBound)
    {
        SDL_Rect newTopShaft = m_TopShaft, newTopTip = m_TopTip, newBottomTip = m_BottomTip, newBottomShaft = m_BottomShaft;
        newTopShaft.x = std::max(m_LowerBound, m_TopShaft.x);
        newTopShaft.w = std::max(m_TopShaft.x + m_TopShaft.w - m_LowerBound, 0);

        newTopTip.x = std::max(m_LowerBound, m_TopTip.x);
        newTopTip.w = m_TopTip.x + m_TopTip.w - m_LowerBound;

        newBottomTip.x = std::max(m_LowerBound, m_BottomTip.x);
        newBottomTip.w = m_BottomTip.x + m_BottomTip.w - m_LowerBound;

        newBottomShaft.x = std::max(m_LowerBound, m_BottomShaft.x);
        newBottomShaft.w = std::max(m_BottomShaft.x + m_BottomShaft.w - m_LowerBound, 0);

        SDL_Rect newShaft = SHAFT_ATLAS_POSITION;
        newShaft.x = SHAFT_ATLAS_POSITION.x + ((newTopShaft.x - m_TopShaft.x) / static_cast<double>(PIPE_WIDTH)) * SHAFT_ATLAS_POSITION.w;
        newShaft.w = SHAFT_ATLAS_POSITION.w - newShaft.x + SHAFT_ATLAS_POSITION.x;

        SDL_Rect newTip = TIP_ATLAS_POSITION;
        //Same principle.
        newTip.x = TIP_ATLAS_POSITION.x + ((newTopTip.x - m_TopTip.x) / static_cast<double>(TIP_WIDTH)) * TIP_ATLAS_POSITION.w;
        newTip.w = TIP_ATLAS_POSITION.w - newTip.x + TIP_ATLAS_POSITION.x;

        SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &newShaft, &newTopShaft);
        SDL_RenderCopyEx(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &newTip, &newTopTip, 0, NULL, SDL_FLIP_VERTICAL);
        SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &newTip, &newBottomTip);
        SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &newShaft, &newBottomShaft);

        m_AppBind.setDrawColor(Colors::BLACK);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &newTopShaft);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &newTopTip);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &newBottomTip);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &newBottomShaft);
    }
    else if(m_ScreenPosition.x < m_UpperBound && m_ScreenPosition.x + m_ScreenPosition.w > m_UpperBound)//Remaining case : pipe is at the right of the screen
    {
        SDL_Rect newTopShaft = m_TopShaft, newTopTip = m_TopTip, newBottomTip = m_BottomTip, newBottomShaft = m_BottomShaft;
        newTopShaft.x = std::min(m_UpperBound, m_TopShaft.x);
        newTopShaft.w = m_UpperBound - newTopShaft.x;

        newTopTip.x = std::min(m_UpperBound, m_TopTip.x);
        newTopTip.w = m_UpperBound - newTopTip.x;

        newBottomTip.x = std::min(m_UpperBound, m_BottomTip.x);
        newBottomTip.w = m_UpperBound - newBottomTip.x;

        newBottomShaft.x = std::min(m_UpperBound, m_BottomShaft.x);
        newBottomShaft.w = m_UpperBound - newBottomShaft.x;

        SDL_Rect newShaft = SHAFT_ATLAS_POSITION;
        newShaft.w *= (newTopShaft.w / static_cast<double>(PIPE_WIDTH));

        SDL_Rect newTip = TIP_ATLAS_POSITION;
        newTip.w *= (newTopTip.w / static_cast<double>(TIP_WIDTH));

        SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &newShaft, &newTopShaft);
        SDL_RenderCopyEx(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &newTip, &newTopTip, 0, NULL, SDL_FLIP_VERTICAL);
        SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &newTip, &newBottomTip);
        SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &newShaft, &newBottomShaft);

        m_AppBind.setDrawColor(Colors::BLACK);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &newTopShaft);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &newTopTip);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &newBottomTip);
        SDL_RenderDrawRect(m_AppBind.getRenderer(), &newBottomShaft);
    }



    showHitbox();

    m_AppBind.restoreRendererSettings();
}

void Pipe::updateRects()
{
    m_TopShaft.y = 0; m_TopShaft.x = m_ScreenPosition.x + SHAFT_OFFSET; m_TopShaft.w = PIPE_WIDTH; m_TopShaft.h = m_BreakPoint - TIP_HEIGHT;
    m_TopTip.x = m_ScreenPosition.x; m_TopTip.y = m_TopShaft.y + m_TopShaft.h; m_TopTip.w = TIP_WIDTH; m_TopTip.h = TIP_HEIGHT;
    m_BottomShaft.x = m_ScreenPosition.x + SHAFT_OFFSET; m_BottomShaft.y = m_BreakPoint + BREAKPOINT_HEIGHT + TIP_HEIGHT; m_BottomShaft.h = PIPE_HEIGHT - m_BottomShaft.y; m_BottomShaft.w = PIPE_WIDTH;
    m_BottomTip.x = m_ScreenPosition.x; m_BottomTip.y = m_BreakPoint + BREAKPOINT_HEIGHT; m_BottomTip.w = TIP_WIDTH; m_BottomTip.h = TIP_HEIGHT;
}

bool Pipe::playerHitPipe(Player const& player)
{
    updateRects();
    SDL_Rect pPos = player.getScreenPosition();
    return (SDL_HasIntersection(&pPos, &m_TopShaft) || SDL_HasIntersection(&pPos, &m_TopTip) || SDL_HasIntersection(&pPos, &m_BottomTip) || SDL_HasIntersection(&pPos, &m_BottomShaft));
}

bool Pipe::playerGotPoints(Player const& player)
{
    if(m_GavePoints){return false;}
    updateRects();
    SDL_Rect pRect = player.getScreenPosition();
    SDL_Point firstCollision = {m_TopTip.x + m_TopTip.w / 2, m_TopTip.y + m_TopTip.h};
    SDL_Point secondCollision = {m_BottomTip.x + m_BottomTip.w / 2, m_BottomTip.y};

    m_GavePoints = SDL_IntersectRectAndLine(&pRect, &firstCollision.x, &firstCollision.y, &secondCollision.x, &secondCollision.y);
    return m_GavePoints;
}

void Pipe::reset()
{
    m_GavePoints = false;
}



