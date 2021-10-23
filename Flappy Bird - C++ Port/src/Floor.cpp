#include "Floor.h"

Floor::Floor(WindowInstance& app, TextureAtlas& atlas, const ScreenSide type)
:
    AbstractGraphics(app, Position::NONE),
    Sprite(app, atlas, SDL_extensions::rectCreator(0, 0, GRASS_WIDTH, GRASS_HEIGHT)),
    m_Scroll(0),
    m_Type(type)
{
    m_ScreenPosition.y = m_AppBind.getWindowHeight() - FLOOR_HEIGHT;
    m_ScreenPosition.h = FLOOR_HEIGHT;
    switch(m_Type)
    {
    case ScreenSides::LEFT:
        m_ScreenPosition.x = 0;
        m_ScreenPosition.w = m_AppBind.getWindowWidth() / 2;
        break;
    case ScreenSides::RIGHT:
        m_ScreenPosition.x = m_AppBind.getWindowWidth() / 2;
        m_ScreenPosition.w = m_AppBind.getWindowWidth() / 2;
        break;
    case ScreenSides::ALL:default: //If specified value is invalid, just set it to all
        m_ScreenPosition.x = 0;
        m_ScreenPosition.w = m_AppBind.getWindowWidth();
        break;
    }
}

Floor::~Floor()
{
    //dtor
}

void Floor::scroll(const uint8_t val)
{
    m_Scroll += val;
    m_Scroll %= EFFECTIVE_GRASS_WIDTH;
}

void Floor::setScroll(const uint8_t val)
{
    m_Scroll = val;
    m_Scroll %= EFFECTIVE_GRASS_WIDTH;
}

void Floor::show() const
{
    m_AppBind.saveRendererSettings();

    /*
    Right-hand side and all use the same rendering technique : start from whatever, and draw slices one by one until destination reached.
    The left-hand side, though, doesn't work like this : it starts from the middle of the screen and moves leftwards, until the end of the screen
    is reached.
    They both rely on the same principle though : if an extra slice is drawn, to ensure nothing looks cut, it'll be drawn out of the screen
    instead of cutting through the other side of the screen.
    */

    const int iterations = (m_ScreenPosition.w / EFFECTIVE_GRASS_WIDTH) + 1;
    switch(m_Type)
    {
    case ScreenSides::LEFT:
        for(int i = 0 ; i < iterations ; i++)
        {
            SDL_Rect tmp;
            tmp.w = EFFECTIVE_GRASS_WIDTH; tmp.x = m_AppBind.getWindowWidth() / 2 - (i + 1) * EFFECTIVE_GRASS_WIDTH - m_Scroll;
            tmp.y = m_ScreenPosition.y; tmp.h = EFFECTIVE_GRASS_HEIGHT;
            SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &m_AtlasPosition, &tmp);
        }
        {
            SDL_Rect tmp = m_ScreenPosition; tmp.w = m_Scroll;
            tmp.h = EFFECTIVE_GRASS_HEIGHT; tmp.x = m_AppBind.getWindowWidth() / 2 - m_Scroll;

            SDL_Rect atlasTmp = m_AtlasPosition; atlasTmp.w *= static_cast<double>(m_Scroll) / EFFECTIVE_GRASS_WIDTH;
            SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &atlasTmp, &tmp);
        }
        break;
    case ScreenSides::RIGHT:case ScreenSides::ALL:default:
        for(int i = 1 ; i < iterations ; i++)
        {
            SDL_Rect tmp;
            tmp.w = m_AtlasPosition.w * FLOOR_SCALE_FACTOR; tmp.x = m_ScreenPosition.x - m_Scroll + i * tmp.w;
            tmp.y = m_ScreenPosition.y; tmp.h = EFFECTIVE_GRASS_HEIGHT;
            SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &m_AtlasPosition, &tmp);
        }
        {
            SDL_Rect tmp = m_ScreenPosition; tmp.w = EFFECTIVE_GRASS_WIDTH - m_Scroll;
            tmp.h = EFFECTIVE_GRASS_HEIGHT;
            SDL_Rect atlasTmp = m_AtlasPosition; atlasTmp.w -= m_Scroll / FLOOR_SCALE_FACTOR; atlasTmp.x += m_Scroll / FLOOR_SCALE_FACTOR;
            SDL_RenderCopy(m_AppBind.getRenderer(), m_Atlas.getAtlas(), &atlasTmp, &tmp);
        }
        break;
    }

    SDL_Rect dirt = SDL_extensions::rectCreator(m_ScreenPosition.x, m_ScreenPosition.y + EFFECTIVE_GRASS_HEIGHT, m_ScreenPosition.w, FLOOR_HEIGHT - EFFECTIVE_GRASS_HEIGHT);
    m_AppBind.setDrawColor(Colors::colorPicker(185, 122, 86, 255));
    SDL_RenderFillRect(m_AppBind.getRenderer(), &dirt);

    m_AppBind.restoreRendererSettings();
}

bool Floor::playerHitFloor(Player const& player) const
{
    SDL_Rect a = player.getScreenPosition();
    return SDL_HasIntersection(&a, &m_ScreenPosition);
}
